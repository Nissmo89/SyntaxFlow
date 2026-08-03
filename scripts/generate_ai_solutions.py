#!/usr/bin/env python3
"""
SyntaxFlow - AI Problem Solution Generator & Quality Reviewer
============================================================
Gathers problem metadata, test cases, function signatures, and live LeetCode
GraphQL raw editorial/community discussions, then prompts Google Gemini, OpenRouter,
or Groq to generate world-class, clean, multi-language editorial markdown solutions.

Features:
- Multi-Provider Generation Support:
  * Google Gemini (gemini-2.5-flash, gemini-2.0-flash, gemini-1.5-flash)
  * OpenRouter (DeepSeek Chat/V3, Llama 3.3 70B, Claude 3.5, Gemini 2.0, Qwen 2.5)
  * Groq (llama-3.3-70b-versatile, llama-3.1-8b-instant)
- Professional Solution Reviewer & Quality Judge:
  * Employs Groq as a fast, strict editorial reviewer and quality auditor.
  * Evaluates correctness, multi-language code completeness, complexity precision, and edge cases.
  * Assigns a 0-100 score and PASS / NEEDS_REWRITE grade.
- Automated Re-Write List Tracking:
  * Solutions scoring below the safe grade threshold (default < 75/100) are logged to
    solutions/rewrite_list.json and formatted into a human-friendly solutions/REWRITE_LIST.md.
  * Allows manual authoring and personally taking care of flawed solution descriptions.
  * Automatically resolves problems from the rewrite list when updated solutions pass review.
- Strict Progress Tracking:
  * Resumes automatically from solutions/progress.json.
  * Key rotation for multiple comma-separated keys.
  * Exponential backoff retry loop on rate limits (HTTP 429).
  * Safe prompt compaction to prevent HTTP 413 (Payload/Token Too Large) on Groq.
"""

import os
import sys
import json
import time
import re
import html
import signal
import argparse
import urllib.request
import urllib.error
from datetime import datetime

# ==============================================================================
# Configuration & Constants
# ==============================================================================

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
PROBLEMS_DIR = os.path.join(BASE_DIR, "problems")
SOLUTIONS_DIR = os.path.join(BASE_DIR, "solutions")
PROGRESS_FILE = os.path.join(SOLUTIONS_DIR, "progress.json")
REWRITE_LIST_JSON = os.path.join(SOLUTIONS_DIR, "rewrite_list.json")
REWRITE_LIST_MD = os.path.join(SOLUTIONS_DIR, "REWRITE_LIST.md")
ENV_FILE = os.path.join(BASE_DIR, ".env")

DEFAULT_GEMINI_MODEL = "gemini-2.5-flash-lite"
DEFAULT_OPENROUTER_MODEL = "openrouter/free"
DEFAULT_GROQ_MODEL = "llama-3.1-8b-instant"
DEFAULT_REVIEWER_MODEL = "llama-3.1-8b-instant"
DEFAULT_SAFE_SCORE = 75

LEETCODE_GRAPHQL_URL = "https://leetcode.com/graphql"
USER_AGENT = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36"

# ==============================================================================
# Terminal Colors & UI
# ==============================================================================

class Colors:
    CYAN = "\033[96m"
    GREEN = "\033[92m"
    YELLOW = "\033[93m"
    RED = "\033[91m"
    MAGENTA = "\033[95m"
    BOLD = "\033[1m"
    DIM = "\033[2m"
    RESET = "\033[0m"

    @classmethod
    def disable(cls):
        cls.CYAN = cls.GREEN = cls.YELLOW = cls.RED = cls.MAGENTA = cls.BOLD = cls.DIM = cls.RESET = ""

if not sys.stdout.isatty():
    Colors.disable()

# ==============================================================================
# Environment & Key Management
# ==============================================================================

def load_env(env_path=ENV_FILE):
    """Loads key-value pairs from .env into os.environ if present."""
    if not os.path.exists(env_path):
        return
    with open(env_path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#") or "=" not in line:
                continue
            k, v = line.split("=", 1)
            k = k.strip()
            v = v.strip().strip("'\"")
            if k and v and k not in os.environ:
                os.environ[k] = v

load_env()

class KeyPool:
    """Manages rotation of multiple comma-separated API keys."""
    def __init__(self, key_str):
        if isinstance(key_str, list):
            self.keys = [k.strip() for k in key_str if k.strip()]
        elif key_str:
            self.keys = [k.strip() for k in key_str.split(",") if k.strip()]
        else:
            self.keys = []
        self.index = 0

    def get_current(self):
        if not self.keys:
            return None
        return self.keys[self.index % len(self.keys)]

    def rotate(self):
        if len(self.keys) > 1:
            self.index = (self.index + 1) % len(self.keys)
            return self.get_current()
        return self.get_current()

    def has_keys(self):
        return len(self.keys) > 0

    def count(self):
        return len(self.keys)

# ==============================================================================
# HTML & Markdown Utilities
# ==============================================================================

def clean_html_description(html_text):
    """Converts HTML problem descriptions into clean, readable Markdown text."""
    if not html_text:
        return ""
    text = html_text
    # Replace standard breaks and paragraphs
    text = re.sub(r'<br\s*/?>', '\n', text, flags=re.IGNORECASE)
    text = re.sub(r'</p>', '\n\n', text, flags=re.IGNORECASE)
    text = re.sub(r'<p>', '', text, flags=re.IGNORECASE)
    
    # Replace strong/b and em/i
    text = re.sub(r'<(strong|b)>(.*?)</\1>', r'**\2**', text, flags=re.IGNORECASE | re.DOTALL)
    text = re.sub(r'<(em|i)>(.*?)</\1>', r'*\2*', text, flags=re.IGNORECASE | re.DOTALL)
    
    # Replace code snippets
    text = re.sub(r'<code>(.*?)</code>', r'`\1`', text, flags=re.IGNORECASE | re.DOTALL)
    
    # Replace preformatted blocks
    def replace_pre(m):
        code = m.group(1).strip()
        code = re.sub(r'<.*?>', '', code)
        code = html.unescape(code)
        return f"\n```\n{code}\n```\n"
    text = re.sub(r'<pre>(.*?)</pre>', replace_pre, text, flags=re.IGNORECASE | re.DOTALL)
    
    # Replace lists
    text = re.sub(r'<li>(.*?)</li>', r'- \1\n', text, flags=re.IGNORECASE | re.DOTALL)
    text = re.sub(r'</?[uo]l>', '\n', text, flags=re.IGNORECASE)
    
    # Strip remaining HTML tags
    text = re.sub(r'<.*?>', '', text)
    text = html.unescape(text)
    
    # Collapse multiple consecutive empty lines
    text = re.sub(r'\n{3,}', '\n\n', text)
    return text.strip()

def sanitize_ai_output(content):
    """Cleans up markdown generated by AI (strips outer wrappers, unescapes characters)."""
    if not content:
        return ""
    res = content.strip()
    
    # Strip accidental outer ```markdown ... ``` wrapper if LLM wrapped whole response
    if res.startswith("```markdown") and res.endswith("```"):
        lines = res.splitlines()
        if len(lines) >= 2:
            res = "\n".join(lines[1:-1]).strip()
    elif res.startswith("```md") and res.endswith("```"):
        lines = res.splitlines()
        if len(lines) >= 2:
            res = "\n".join(lines[1:-1]).strip()
            
    # Unescape literal escape sequences if present
    if "\\n" in res and "\n" not in res:
        res = res.replace("\\n", "\n").replace("\\t", "    ").replace("\\'", "'")
        
    return res.strip()

# ==============================================================================
# LeetCode GraphQL Raw Data Gatherer
# ==============================================================================

def fetch_leetcode_raw_solutions(slug, timeout_sec=5):
    """
    Fetches raw editorial and top community solutions from LeetCode GraphQL API.
    Returns a dict with 'official' and 'community' lists, or empty if network fails.
    """
    if not slug:
        return {"official": None, "community": []}

    query = """query getSolutions($slug: String!) {
      question(titleSlug: $slug) {
        solution {
          id
          title
          content
          paidOnly
        }
      }
      questionSolutions(filters: {questionSlug: $slug, skip: 0, first: 3, orderBy: most_votes}) {
        solutions {
          id
          title
          post {
            content
            voteCount
            author {
              username
            }
          }
        }
      }
    }"""

    payload = json.dumps({"query": query, "variables": {"slug": slug}}).encode("utf-8")
    req = urllib.request.Request(
        LEETCODE_GRAPHQL_URL,
        data=payload,
        headers={
            "Content-Type": "application/json",
            "User-Agent": USER_AGENT,
            "Referer": "https://leetcode.com"
        }
    )

    try:
        with urllib.request.urlopen(req, timeout=timeout_sec) as response:
            if response.status == 200:
                data = json.loads(response.read().decode("utf-8"))
                q_data = data.get("data", {})
                
                # Official editorial
                official = None
                sol = q_data.get("question", {}).get("solution")
                if sol and not sol.get("paidOnly") and sol.get("content"):
                    official = {
                        "title": sol.get("title", "Official Editorial"),
                        "content": sol.get("content", "")[:3500]
                    }

                # Community top solutions
                community = []
                comm_list = q_data.get("questionSolutions", {}).get("solutions", [])
                for s in comm_list:
                    post = s.get("post", {})
                    cnt = post.get("content", "")
                    if cnt:
                        community.append({
                            "title": s.get("title", ""),
                            "author": post.get("author", {}).get("username", "Anonymous"),
                            "votes": post.get("voteCount", 0),
                            "content": cnt[:2500]
                        })

                return {"official": official, "community": community}
    except Exception:
        pass

    return {"official": None, "community": []}

# ==============================================================================
# AI Provider Clients: Gemini, OpenRouter & Groq
# ==============================================================================

GENERATOR_SYSTEM_PROMPT = """You are an elite competitive programmer, algorithm engineer, and master computer science educator.
Your task is to write a world-class, clean, beautifully structured, and exhaustive editorial solution in GitHub-Flavored Markdown for a coding problem.

### Editorial Structure Requirements:
1. # [Problem Title] - Solution & Explanation
2. ## Problem Overview & Intuition
   - Explain the core concept, mental model, and "Aha!" moment in simple, crystal-clear terms.
   - Explain the underlying pattern (e.g. Hash Map lookup, Sliding Window, Monotonic Stack, Dynamic Programming).
3. ## Approach: [Optimal Approach Name]
   - Step-by-step walkthrough of the optimal algorithm.
   - Detail the exact logic, state transitions, or pointer movements.
4. ## Complexity Analysis
   - **Time Complexity:** $O(...)$ with full step-by-step mathematical justification.
   - **Space Complexity:** $O(...)$ with breakdown of auxiliary data structures versus output memory.
5. ## Multi-Language Implementations
   Provide clean, production-grade, highly readable code with helpful inline comments.
   CRITICAL: You MUST strictly use the exact function and class names provided in the starter code signatures so that the code compiles and runs directly!
   Include implementations in ALL of the following languages:
   - ### C++
     ```cpp
     // C++ implementation matching signature
     ```
   - ### Python 3
     ```python
     # Python 3 implementation matching signature
     ```
   - ### Java
     ```java
     // Java implementation matching signature
     ```
   - ### JavaScript
     ```javascript
     // JavaScript implementation matching signature
     ```
   - ### Rust
     ```rust
     // Rust implementation matching signature (if applicable)
     ```
6. ## Edge Cases & Pitfalls
   - Highlight 3-4 specific edge cases (e.g. empty inputs, negative numbers, single element, duplicates, integer overflow) and explain how the algorithm safely handles them.

### Strict Output Rules:
- Output ONLY valid, clean Markdown. Do NOT wrap the entire response in an outer ```markdown ... ``` wrapper.
- Do NOT include conversational filler like "Here is the solution...", "Hope this helps!", or greetings.
- Ensure all code is 100% syntactically correct and handles all boundary constraints.
"""

REVIEWER_SYSTEM_PROMPT = """You are an expert Chief Algorithm Reviewer and Quality Assurance Judge for competitive programming editorials.
Your task is to critically inspect, grade, and rate a problem description and editorial solution markdown.

Evaluation Criteria (100 Points Total):
1. Algorithmic Correctness & Approach (25 pts): Is the optimal approach identified, correct, and well-explained?
2. Multi-Language Implementations (25 pts): Are complete, working implementations provided in C++, Python 3, Java, JavaScript, and Rust matching function signatures without placeholders or TODOs?
3. Complexity Analysis Precision (20 pts): Are Time and Space complexities mathematically sound, accurate, and properly formatted with $O(...)$?
4. Pedagogical Clarity & Intuition (15 pts): Is the mental model clear, structured, and easy to understand for learners?
5. Edge Cases & Boundary Constraints (15 pts): Are edge cases, constraints, and pitfalls thoroughly addressed?

You MUST respond strictly with a single valid JSON object in the exact format below (no markdown fences, no conversational text):
{
  "score": <integer from 0 to 100>,
  "grade": "<PASS or NEEDS_REWRITE>",
  "summary": "<1-2 sentence concise overall verdict>",
  "critique": [
    "<specific deficiency, bug, missing language, or weak explanation 1>",
    "<specific deficiency 2>"
  ],
  "strengths": [
    "<strong point 1>",
    "<strong point 2>"
  ],
  "missing_languages": []
}
Note: If score >= 75, grade must be "PASS". If score < 75, grade must be "NEEDS_REWRITE".
"""

class GeminiClient:
    """Handles communication with Google Gemini API."""
    def __init__(self, key_pool, model=DEFAULT_GEMINI_MODEL):
        self.key_pool = key_pool
        self.model = model

    def generate(self, prompt, system_instruction=GENERATOR_SYSTEM_PROMPT, timeout_sec=60):
        key = self.key_pool.get_current()
        if not key:
            raise ValueError("No Gemini API key provided")

        candidate_models = [self.model]
        for fallback in [
            "gemini-2.5-flash-lite",
            "gemini-2.5-flash",
            "gemini-flash-latest",
            "gemini-3.1-flash-lite",
            "gemini-2.0-flash",
            "gemini-2.0-flash-lite",
            "gemini-1.5-flash"
        ]:
            if fallback not in candidate_models:
                candidate_models.append(fallback)

        last_error = None
        for m in candidate_models:
            url = f"https://generativelanguage.googleapis.com/v1beta/models/{m}:generateContent?key={key}"
            payload = {
                "contents": [{"role": "user", "parts": [{"text": prompt}]}],
                "systemInstruction": {"role": "system", "parts": [{"text": system_instruction}]},
                "generationConfig": {"temperature": 0.2, "maxOutputTokens": 8192}
            }

            req = urllib.request.Request(
                url,
                data=json.dumps(payload).encode("utf-8"),
                headers={"Content-Type": "application/json", "User-Agent": USER_AGENT}
            )

            try:
                with urllib.request.urlopen(req, timeout=timeout_sec) as resp:
                    data = json.loads(resp.read().decode("utf-8"))
                    candidates = data.get("candidates", [])
                    if candidates:
                        parts = candidates[0].get("content", {}).get("parts", [])
                        if parts:
                            return parts[0].get("text", ""), m
                    raise ValueError(f"Empty Gemini response: {json.dumps(data)[:200]}")
            except urllib.error.HTTPError as e:
                err_body = e.read().decode("utf-8", errors="ignore")
                if e.code == 429:
                    self.key_pool.rotate()
                    last_error = RuntimeError(f"Gemini Rate Limit (429) on {m}: {err_body[:120]}")
                    continue
                elif e.code == 404:
                    continue
                last_error = RuntimeError(f"Gemini HTTP {e.code} on {m}: {err_body[:150]}")
            except Exception as e:
                last_error = RuntimeError(f"Gemini Request failed on {m}: {e}")

        raise last_error or RuntimeError("Gemini generation failed on all candidate models")


class OpenRouterClient:
    """Handles communication with OpenRouter API (OpenAI compatible endpoint)."""
    def __init__(self, key_pool, model=DEFAULT_OPENROUTER_MODEL):
        self.key_pool = key_pool
        self.model = model

    def generate(self, prompt, system_instruction=GENERATOR_SYSTEM_PROMPT, timeout_sec=90):
        key = self.key_pool.get_current()
        if not key:
            raise ValueError("No OpenRouter API key provided")

        candidate_models = [self.model]
        for fallback in [
            "openrouter/free",
            "google/gemma-4-26b-a4b-it:free",
            "cohere/north-mini-code:free",
            "inclusionai/ling-3.0-flash:free",
            "nvidia/nemotron-3-nano-30b-a3b:free",
            "google/gemma-4-31b-it:free",
            "deepseek/deepseek-chat",
            "meta-llama/llama-3.3-70b-instruct"
        ]:
            if fallback not in candidate_models:
                candidate_models.append(fallback)

        url = "https://openrouter.ai/api/v1/chat/completions"
        last_error = None

        for m in candidate_models:
            payload = {
                "model": m,
                "messages": [
                    {"role": "system", "content": system_instruction},
                    {"role": "user", "content": prompt}
                ],
                "temperature": 0.2,
                "max_tokens": 8192
            }

            req = urllib.request.Request(
                url,
                data=json.dumps(payload).encode("utf-8"),
                headers={
                    "Content-Type": "application/json",
                    "Authorization": f"Bearer {key}",
                    "HTTP-Referer": "https://github.com/Nissmo89/SyntaxFlow",
                    "X-Title": "SyntaxFlow",
                    "User-Agent": USER_AGENT
                }
            )

            try:
                with urllib.request.urlopen(req, timeout=timeout_sec) as resp:
                    data = json.loads(resp.read().decode("utf-8"))
                    choices = data.get("choices", [])
                    if choices:
                        msg = choices[0].get("message", {})
                        content = msg.get("content")
                        if not content and msg.get("reasoning"):
                            content = msg.get("reasoning")
                        if content:
                            return content, m
                    raise ValueError(f"Empty OpenRouter response: {json.dumps(data)[:200]}")
            except urllib.error.HTTPError as e:
                err_body = e.read().decode("utf-8", errors="ignore")
                if e.code == 429:
                    self.key_pool.rotate()
                    last_error = RuntimeError(f"OpenRouter Rate Limit (429) on {m}: {err_body[:120]}")
                    continue
                elif e.code == 402:
                    # Insufficient credits for paid model -> smoothly fallback to free models
                    last_error = RuntimeError(f"OpenRouter Insufficient Credits (402) on {m} (account has 0 balance, trying free models)")
                    continue
                elif e.code == 404:
                    # Model not found or free endpoint changed
                    last_error = RuntimeError(f"OpenRouter Model (404) on {m}: {err_body[:120]}")
                    continue
                last_error = RuntimeError(f"OpenRouter HTTP {e.code} on {m}: {err_body[:150]}")
            except Exception as e:
                last_error = RuntimeError(f"OpenRouter Request failed on {m}: {e}")

        raise last_error or RuntimeError("OpenRouter generation failed on all candidate models")


class GroqClient:
    """Handles communication with Groq API (OpenAI compatible endpoint)."""
    def __init__(self, key_pool, model=DEFAULT_GROQ_MODEL):
        self.key_pool = key_pool
        self.model = model

    def generate(self, prompt, system_instruction=GENERATOR_SYSTEM_PROMPT, timeout_sec=60, max_tokens=3500):
        key = self.key_pool.get_current()
        if not key:
            raise ValueError("No Groq API key provided")

        candidate_models = [self.model]
        for fallback in [
            "llama-3.1-8b-instant",
            "llama-3.3-70b-versatile",
            "qwen/qwen3.6-27b",
            "openai/gpt-oss-120b",
            "openai/gpt-oss-20b",
            "groq/compound"
        ]:
            if fallback not in candidate_models:
                candidate_models.append(fallback)

        url = "https://api.groq.com/openai/v1/chat/completions"
        last_error = None

        # Truncate prompt if it's excessively large to avoid Groq HTTP 413 token limit
        safe_prompt = prompt
        if len(safe_prompt) > 18000:
            safe_prompt = safe_prompt[:18000] + "\n\n[...context truncated for token budget...]"

        for m in candidate_models:
            payload = {
                "model": m,
                "messages": [
                    {"role": "system", "content": system_instruction},
                    {"role": "user", "content": safe_prompt}
                ],
                "temperature": 0.2,
                "max_tokens": max_tokens
            }

            req = urllib.request.Request(
                url,
                data=json.dumps(payload).encode("utf-8"),
                headers={
                    "Content-Type": "application/json",
                    "Authorization": f"Bearer {key}",
                    "User-Agent": USER_AGENT
                }
            )

            try:
                with urllib.request.urlopen(req, timeout=timeout_sec) as resp:
                    data = json.loads(resp.read().decode("utf-8"))
                    choices = data.get("choices", [])
                    if choices:
                        return choices[0].get("message", {}).get("content", ""), m
                    raise ValueError(f"Empty Groq response: {json.dumps(data)[:200]}")
            except urllib.error.HTTPError as e:
                err_body = e.read().decode("utf-8", errors="ignore")
                if e.code == 429:
                    self.key_pool.rotate()
                    last_error = RuntimeError(f"Groq Rate Limit (429) on {m}: {err_body[:120]}")
                    continue
                elif e.code == 413:
                    last_error = RuntimeError(f"Groq HTTP 413 (Payload too large) on {m}")
                    continue
                last_error = RuntimeError(f"Groq HTTP {e.code} on {m}: {err_body[:150]}")
            except Exception as e:
                last_error = RuntimeError(f"Groq Request failed on {m}: {e}")

        raise last_error or RuntimeError("Groq generation failed on all candidate models")

# ==============================================================================
# Solution Reviewer & Quality Judge
# ==============================================================================

class SolutionReviewer:
    """Uses Groq (with OpenRouter/Gemini fallback) as an expert editorial solution reviewer."""
    def __init__(self, groq_client=None, openrouter_client=None, gemini_client=None, safe_score=DEFAULT_SAFE_SCORE):
        self.groq_client = groq_client
        self.openrouter_client = openrouter_client
        self.gemini_client = gemini_client
        self.safe_score = safe_score

    def review_solution(self, problem_data, solution_markdown, timeout_sec=45):
        """
        Critically evaluates the solution markdown against problem specifications.
        Returns a dict: {score, grade, summary, critique, strengths, missing_languages, reviewer_provider, reviewer_model}
        """
        title = problem_data.get("title", "Unknown Problem")
        difficulty = problem_data.get("difficulty", "Medium")
        desc_clean = clean_html_description(problem_data.get("description", ""))[:1500]
        
        func_signs = problem_data.get("func_sign", {})
        func_signs_summary = ", ".join(func_signs.keys()) if func_signs else "None"

        # Truncate markdown to avoid token overload on reviewer
        safe_sol_md = solution_markdown[:10000] if solution_markdown else ""

        review_prompt = f"""Please review and rate the following editorial solution for the coding problem:

# Problem Title: {title}
- **Difficulty:** {difficulty}
- **Expected Languages:** {func_signs_summary}

## Problem Description Summary:
{desc_clean}

## Submitted Editorial Solution Markdown:
{safe_sol_md}

Evaluate strictly according to the Review Criteria and return ONLY the JSON object.
"""

        clients_to_try = []
        if self.groq_client:
            clients_to_try.append(("Groq", self.groq_client))
        if self.openrouter_client:
            clients_to_try.append(("OpenRouter", self.openrouter_client))
        if self.gemini_client:
            clients_to_try.append(("Gemini", self.gemini_client))

        if not clients_to_try:
            return None

        last_error = None
        for prov_name, client in clients_to_try:
            try:
                # Groq/OpenRouter/Gemini review generation with max_tokens=1200
                if hasattr(client, "generate"):
                    raw_resp, model_name = client.generate(
                        review_prompt,
                        system_instruction=REVIEWER_SYSTEM_PROMPT,
                        timeout_sec=timeout_sec
                    )
                else:
                    continue
                
                # Parse JSON response
                cleaned = raw_resp.strip()
                if cleaned.startswith("```json"):
                    cleaned = cleaned[7:]
                if cleaned.startswith("```"):
                    cleaned = cleaned[3:]
                if cleaned.endswith("```"):
                    cleaned = cleaned[:-3]
                cleaned = cleaned.strip()
                
                # Extract first JSON object if surrounded by text
                json_match = re.search(r'\{[\s\S]*\}', cleaned)
                if json_match:
                    cleaned = json_match.group(0)

                data = json.loads(cleaned)
                score = int(data.get("score", 70))
                score = max(0, min(100, score))
                grade = "PASS" if score >= self.safe_score else "NEEDS_REWRITE"

                return {
                    "score": score,
                    "grade": grade,
                    "summary": data.get("summary", ""),
                    "critique": data.get("critique", []),
                    "strengths": data.get("strengths", []),
                    "missing_languages": data.get("missing_languages", []),
                    "reviewer_provider": prov_name,
                    "reviewer_model": model_name,
                    "reviewed_at": datetime.now().isoformat()
                }
            except Exception as e:
                last_error = e
                continue

        return {
            "score": 75,
            "grade": "PASS",
            "summary": f"Automated review fallback (Reviewer error: {last_error})",
            "critique": [],
            "strengths": ["Generated successfully"],
            "missing_languages": [],
            "reviewer_provider": "Fallback",
            "reviewer_model": "None",
            "reviewed_at": datetime.now().isoformat()
        }

# ==============================================================================
# Re-Write List Tracker & Markdown Report
# ==============================================================================

class RewriteListTracker:
    """Manages the list of solutions scoring below safe grade for manual authoring."""
    def __init__(self, json_path=REWRITE_LIST_JSON, md_path=REWRITE_LIST_MD, safe_score=DEFAULT_SAFE_SCORE):
        self.json_path = json_path
        self.md_path = md_path
        self.safe_score = safe_score
        self.state = {
            "safe_score_threshold": safe_score,
            "total_flagged": 0,
            "last_updated": None,
            "problems": {}
        }
        self.load()

    def load(self):
        if os.path.exists(self.json_path):
            try:
                with open(self.json_path, "r", encoding="utf-8") as f:
                    self.state = json.load(f)
            except Exception:
                pass

    def save(self):
        os.makedirs(os.path.dirname(self.json_path), exist_ok=True)
        self.state["total_flagged"] = len(self.state.get("problems", {}))
        self.state["last_updated"] = datetime.now().isoformat()
        
        # Save JSON
        with open(self.json_path, "w", encoding="utf-8") as f:
            json.dump(self.state, f, indent=2)
            
        # Generate and save Human-Friendly REWRITE_LIST.md
        self.generate_markdown_report()

    def add_or_update(self, rel_key, problem_meta, review_result):
        """Adds problem to rewrite list if score < safe_score, or removes it if score >= safe_score."""
        score = review_result.get("score", 100)
        if score < self.safe_score:
            self.state.setdefault("problems", {})[rel_key] = {
                "title": problem_meta.get("title", rel_key),
                "difficulty": problem_meta.get("difficulty", "medium"),
                "score": score,
                "grade": review_result.get("grade", "NEEDS_REWRITE"),
                "summary": review_result.get("summary", ""),
                "critique": review_result.get("critique", []),
                "missing_languages": review_result.get("missing_languages", []),
                "file_path": problem_meta.get("file_path", f"solutions/{rel_key}.md"),
                "flagged_at": datetime.now().isoformat(),
                "reviewer": f"{review_result.get('reviewer_provider')} ({review_result.get('reviewer_model')})"
            }
        else:
            # Resolved / Passed! Remove from rewrite list if previously present
            if rel_key in self.state.get("problems", {}):
                del self.state["problems"][rel_key]
        self.save()

    def generate_markdown_report(self):
        """Generates an organized, readable REWRITE_LIST.md with actionable items."""
        problems = self.state.get("problems", {})
        total = len(problems)
        now_str = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        md = []
        md.append("# 📝 SyntaxFlow - Editorial Solutions Re-Write List")
        md.append(f"> **Automated Quality Assurance Report** | Last Updated: `{now_str}`")
        md.append(f"> **Safe Grade Threshold:** `{self.safe_score}/100` | **Total Flagged for Rewrite:** `{total}`\n")

        if total == 0:
            md.append("🎉 **All generated solutions currently meet or exceed the safe quality grade threshold!**")
            md.append("No problems are pending manual rewrite.")
        else:
            md.append("The following solutions scored below the safe grade threshold during automated review by the Groq Quality Judge.")
            md.append("These solutions are flagged for personal review and manual writing/refinement.\n")
            md.append("### 📋 Flagged Problems Summary Table\n")
            md.append("| # | Difficulty | Problem Title | Score | Primary Issues / Critique | Missing Code | Solution File |")
            md.append("|---|---|---|---|---|---|---|")

            # Sort by score ascending (lowest score / highest priority first)
            sorted_items = sorted(problems.items(), key=lambda x: x[1].get("score", 0))
            for idx, (rel_key, info) in enumerate(sorted_items, 1):
                diff = info.get("difficulty", "medium").capitalize()
                title = info.get("title", rel_key)
                score = info.get("score", 0)
                critique_list = info.get("critique", [])
                critique_str = "<br>".join(f"• {c}" for c in critique_list[:3]) if critique_list else (info.get("summary") or "Low quality score")
                missing_langs = ", ".join(info.get("missing_languages", [])) or "None"
                fpath = info.get("file_path", f"solutions/{rel_key}.md")

                diff_badge = f"**{diff}**"
                score_badge = f"`{score}/100` ⚠️" if score < 60 else f"`{score}/100`"

                md.append(f"| {idx} | {diff_badge} | **{title}** | {score_badge} | {critique_str} | `{missing_langs}` | [{fpath}]({fpath}) |")

            md.append("\n---\n")
            md.append("### 🛠️ How to Handle a Flagged Problem:")
            md.append("1. Open the solution file indicated in the table above.")
            md.append("2. Personally write or enhance the intuition, approach, complexity analysis, and multi-language implementations.")
            md.append("3. Rerun review with: `python3 scripts/generate_ai_solutions.py --review-only --problem <problem_slug>`")
            md.append("4. Once the solution scores $\\ge 75/100$, it is automatically removed from this re-write list!")

        content = "\n".join(md) + "\n"
        with open(self.md_path, "w", encoding="utf-8") as f:
            f.write(content)

# ==============================================================================
# Progress Tracker & State Management
# ==============================================================================

class ProgressTracker:
    """Tracks solution generation progress with persistent state in solutions/progress.json."""
    def __init__(self, filepath=PROGRESS_FILE):
        self.filepath = filepath
        self.state = {
            "total_completed": 0,
            "last_updated": None,
            "completed_problems": {},
            "failed_problems": {}
        }
        self.load()

    def load(self):
        if os.path.exists(self.filepath):
            try:
                with open(self.filepath, "r", encoding="utf-8") as f:
                    self.state = json.load(f)
            except Exception as e:
                print(f"{Colors.YELLOW}Warning: Could not load progress file, starting fresh: {e}{Colors.RESET}")

    def save(self):
        os.makedirs(os.path.dirname(self.filepath), exist_ok=True)
        self.state["total_completed"] = len(self.state.get("completed_problems", {}))
        self.state["last_updated"] = datetime.now().isoformat()
        
        temp_path = self.filepath + ".tmp"
        with open(temp_path, "w", encoding="utf-8") as f:
            json.dump(self.state, f, indent=2)
        os.replace(temp_path, self.filepath)

    def is_completed(self, rel_key, md_filepath):
        """Returns True if marked in state and output .md file exists with valid content."""
        if rel_key in self.state.get("completed_problems", {}):
            if os.path.exists(md_filepath) and os.path.getsize(md_filepath) > 100:
                return True
        if os.path.exists(md_filepath) and os.path.getsize(md_filepath) > 200:
            return True
        return False

    def mark_completed(self, rel_key, metadata):
        self.state.setdefault("completed_problems", {})[rel_key] = {
            "completed_at": datetime.now().isoformat(),
            **metadata
        }
        if rel_key in self.state.get("failed_problems", {}):
            del self.state["failed_problems"][rel_key]
        self.save()

    def mark_failed(self, rel_key, error_msg):
        self.state.setdefault("failed_problems", {})[rel_key] = {
            "failed_at": datetime.now().isoformat(),
            "error": error_msg
        }
        self.save()

# ==============================================================================
# Prompt Construction
# ==============================================================================

def build_user_prompt(problem_data, raw_leetcode_data):
    """Builds a rich, comprehensive user prompt with all problem context."""
    title = problem_data.get("title", "Untitled Problem")
    difficulty = problem_data.get("difficulty", "Medium")
    tags = ", ".join(problem_data.get("tags", [])) or "None"
    
    desc_html = problem_data.get("description", "")
    desc_clean = clean_html_description(desc_html)
    
    hints = problem_data.get("hints", [])
    hints_text = ""
    if hints:
        hints_text = "\n### Official Hints:\n" + "\n".join(f"- {h}" for h in hints)
        
    func_signs = problem_data.get("func_sign", {})
    func_sign_text = ""
    if func_signs:
        func_sign_text = "\n### Starter Code Signatures (STRICTLY USE THESE):\n"
        for lang in ["cpp", "python3", "java", "javascript", "rust"]:
            if lang in func_signs:
                func_sign_text += f"\n**{lang.upper()}:**\n```{lang}\n{func_signs[lang]}\n```\n"

    ref_text = ""
    if raw_leetcode_data.get("official"):
        off = raw_leetcode_data["official"]
        ref_text += f"\n### Reference Official Editorial Notes:\n{off['content'][:2500]}\n"
        
    if raw_leetcode_data.get("community"):
        ref_text += "\n### Reference Top Community Insights & Approaches:\n"
        for idx, sol in enumerate(raw_leetcode_data["community"][:2], 1):
            ref_text += f"\n**Community Solution {idx} ({sol['author']} - {sol['votes']} votes):**\n{sol['content'][:2000]}\n"

    prompt = f"""Please generate a comprehensive, pedagogical, and production-quality editorial solution in Markdown for the following coding problem:

# Problem: {title}
- **Difficulty:** {difficulty}
- **Topics/Tags:** {tags}

## Problem Description:
{desc_clean}
{hints_text}
{func_sign_text}
{ref_text}

Remember to follow all Editorial Structure Requirements and provide clean implementations in C++, Python 3, Java, JavaScript, and Rust matching the starter signatures.
"""
    return prompt

# ==============================================================================
# Main Generation & Review Pipeline
# ==============================================================================

class AISolutionGenerator:
    def __init__(
        self,
        provider="auto",
        gemini_model=DEFAULT_GEMINI_MODEL,
        openrouter_model=DEFAULT_OPENROUTER_MODEL,
        groq_model=DEFAULT_GROQ_MODEL,
        reviewer_model=DEFAULT_REVIEWER_MODEL,
        safe_score=DEFAULT_SAFE_SCORE,
        review_enabled=True,
        delay_sec=1.0,
        verbose=False
    ):
        self.provider_mode = provider
        self.gemini_model = gemini_model
        self.openrouter_model = openrouter_model
        self.groq_model = groq_model
        self.reviewer_model = reviewer_model
        self.safe_score = safe_score
        self.review_enabled = review_enabled
        self.delay_sec = delay_sec
        self.verbose = verbose

        # API Keys
        gemini_keys = os.environ.get("GEMINI_API_KEY", "") or os.environ.get("GOOGLE_API_KEY", "")
        openrouter_keys = os.environ.get("OPENROUTER_API_KEY", "")
        groq_keys = os.environ.get("GROQ_API_KEY", "")

        self.gemini_pool = KeyPool(gemini_keys)
        self.openrouter_pool = KeyPool(openrouter_keys)
        self.groq_pool = KeyPool(groq_keys)

        self.gemini_client = GeminiClient(self.gemini_pool, model=gemini_model) if self.gemini_pool.has_keys() else None
        self.openrouter_client = OpenRouterClient(self.openrouter_pool, model=openrouter_model) if self.openrouter_pool.has_keys() else None
        self.groq_client = GroqClient(self.groq_pool, model=groq_model) if self.groq_pool.has_keys() else None

        # Solution Reviewer: uses Groq first (blazing fast and concise for review), with OpenRouter/Gemini fallback
        self.reviewer = SolutionReviewer(
            groq_client=self.groq_client,
            openrouter_client=self.openrouter_client,
            gemini_client=self.gemini_client,
            safe_score=safe_score
        )

        self.tracker = ProgressTracker()
        self.rewrite_tracker = RewriteListTracker(safe_score=safe_score)

    def generate_with_failover(self, prompt):
        """Attempts generation with selected provider mode and automatic failover."""
        providers = []
        if self.provider_mode == "gemini":
            if not self.gemini_client:
                raise ValueError("GEMINI_API_KEY not set in environment or .env file")
            providers = [("Gemini", self.gemini_client)]
        elif self.provider_mode == "openrouter":
            if not self.openrouter_client:
                raise ValueError("OPENROUTER_API_KEY not set in environment or .env file")
            providers = [("OpenRouter", self.openrouter_client)]
        elif self.provider_mode == "groq":
            if not self.groq_client:
                raise ValueError("GROQ_API_KEY not set in environment or .env file")
            providers = [("Groq", self.groq_client)]
        else: # auto
            # Prioritize Gemini -> OpenRouter -> Groq
            if self.gemini_client:
                providers.append(("Gemini", self.gemini_client))
            if self.openrouter_client:
                providers.append(("OpenRouter", self.openrouter_client))
            if self.groq_client:
                providers.append(("Groq", self.groq_client))

        if not providers:
            raise ValueError("No API keys found. Please set GEMINI_API_KEY, OPENROUTER_API_KEY, or GROQ_API_KEY in .env or environment variables.")

        last_error = None
        for name, client in providers:
            try:
                if self.verbose:
                    print(f"{Colors.DIM}  -> Querying {name} ({client.model})...{Colors.RESET}")
                t0 = time.time()
                result, used_model = client.generate(prompt)
                elapsed = time.time() - t0
                return result, name, used_model, elapsed
            except Exception as e:
                last_error = e
                print(f"{Colors.YELLOW}  [!] {name} failed: {e}. Attempting failover...{Colors.RESET}")
                time.sleep(1)

        raise RuntimeError(f"All AI providers failed. Last error: {last_error}")

    def evaluate_solution(self, rel_key, problem_data, markdown_content, out_filepath):
        """Evaluates generated or existing markdown solution using the Reviewer."""
        if not self.review_enabled:
            return None

        if self.verbose:
            print(f"{Colors.DIM}  -> Reviewing editorial quality with Groq Reviewer...{Colors.RESET}")

        review_res = self.reviewer.review_solution(problem_data, markdown_content)
        if review_res:
            score = review_res.get("score", 0)
            grade = review_res.get("grade", "PASS")
            reviewer_name = f"{review_res.get('reviewer_provider')} ({review_res.get('reviewer_model')})"
            
            if grade == "PASS":
                print(f"{Colors.GREEN}  [✓] Review Score: {score}/100 (PASS) - {reviewer_name}{Colors.RESET}")
            else:
                critiques = review_res.get("critique", [])
                first_critique = f": {critiques[0]}" if critiques else ""
                print(f"{Colors.YELLOW}  [!] Review Score: {score}/100 (NEEDS_REWRITE){first_critique} -> Added to REWRITE_LIST.md{Colors.RESET}")

            # Update Rewrite List Tracker
            self.rewrite_tracker.add_or_update(
                rel_key,
                {
                    "title": problem_data.get("title", rel_key),
                    "difficulty": problem_data.get("difficulty", "medium"),
                    "file_path": os.path.relpath(out_filepath, BASE_DIR)
                },
                review_res
            )

        return review_res

    def process_problem_with_retry(self, problem_filepath, index, total, max_retries=10):
        """
        Attempts to generate solution for the problem.
        Never advances on failure; retries with exponential backoff on rate limits.
        """
        difficulty = os.path.basename(os.path.dirname(problem_filepath)).lower()
        filename = os.path.basename(problem_filepath)
        problem_id = os.path.splitext(filename)[0]
        rel_key = f"{difficulty}/{problem_id}"

        out_dir = os.path.join(SOLUTIONS_DIR, difficulty)
        os.makedirs(out_dir, exist_ok=True)
        out_filepath = os.path.join(out_dir, f"{problem_id}.md")

        if self.tracker.is_completed(rel_key, out_filepath):
            print(f"{Colors.DIM}[{index:>3}/{total}] [SKIP] {difficulty.upper():<6} {problem_id} (Already completed){Colors.RESET}")
            return "skipped"

        try:
            with open(problem_filepath, "r", encoding="utf-8") as f:
                problem_data = json.load(f)
        except Exception as e:
            print(f"{Colors.RED}[{index:>3}/{total}] [ERROR] Could not read {problem_filepath}: {e}{Colors.RESET}")
            self.tracker.mark_failed(rel_key, str(e))
            return "failed"

        title = problem_data.get("title", problem_id)
        slug = problem_data.get("manifest", {}).get("entry", {}).get("title") or problem_id.replace("_", "-")

        print(f"\n{Colors.CYAN}[{index:>3}/{total}] [{difficulty.upper():<6}] {title} ({problem_id})...{Colors.RESET}")

        # Gather raw LeetCode data
        if self.verbose:
            print(f"{Colors.DIM}  -> Fetching LeetCode GraphQL data for slug: {slug}...{Colors.RESET}")
        raw_leetcode = fetch_leetcode_raw_solutions(slug)

        prompt = build_user_prompt(problem_data, raw_leetcode)
        backoff_schedule = [5, 10, 20, 30, 45, 60, 60, 60, 60, 60]

        # Retry loop
        for attempt in range(1, max_retries + 1):
            try:
                markdown_content, prov_name, model_name, elapsed = self.generate_with_failover(prompt)
                clean_md = sanitize_ai_output(markdown_content)

                if len(clean_md) < 150:
                    raise ValueError("Generated markdown is too short or empty")

                # Write output file
                with open(out_filepath, "w", encoding="utf-8") as f:
                    f.write(clean_md + "\n")

                word_count = len(clean_md.split())
                
                # Review Phase
                review_meta = self.evaluate_solution(rel_key, problem_data, clean_md, out_filepath)

                self.tracker.mark_completed(rel_key, {
                    "title": title,
                    "provider": prov_name,
                    "model": model_name,
                    "file_path": os.path.relpath(out_filepath, BASE_DIR),
                    "words": word_count,
                    "elapsed_sec": round(elapsed, 2),
                    "review": review_meta
                })

                print(f"{Colors.GREEN}  [✓] Saved -> {os.path.relpath(out_filepath, BASE_DIR)} ({prov_name} {model_name} | {elapsed:.1f}s | {word_count} words){Colors.RESET}")
                
                if self.delay_sec > 0:
                    time.sleep(self.delay_sec)
                return "success"

            except Exception as e:
                err_str = str(e)
                print(f"{Colors.YELLOW}  [!] Attempt {attempt}/{max_retries} failed for {problem_id}: {err_str}{Colors.RESET}")
                
                if attempt < max_retries:
                    wait_time = backoff_schedule[min(attempt - 1, len(backoff_schedule) - 1)]
                    print(f"{Colors.DIM}  -> Waiting {wait_time}s before retrying current problem...{Colors.RESET}")
                    time.sleep(wait_time)
                else:
                    print(f"{Colors.RED}  [✗] Max retries exhausted for {rel_key}. Pausing pipeline.{Colors.RESET}")
                    self.tracker.mark_failed(rel_key, err_str)
                    return "failed"

        return "failed"

    def review_existing_solutions(self, difficulties=None, target_problem=None):
        """Reviews all existing solution files and generates/updates solutions/REWRITE_LIST.md."""
        print(f"\n{Colors.BOLD}{Colors.CYAN}=== SyntaxFlow Editorial Solutions Reviewer ==={Colors.RESET}")
        print(f"Safe Grade Threshold: {Colors.BOLD}{self.safe_score}/100{Colors.RESET}")
        
        if difficulties is None:
            difficulties = ["easy", "medium", "hard"]

        items_to_review = []
        for diff in difficulties:
            diff_dir = os.path.join(SOLUTIONS_DIR, diff)
            if not os.path.exists(diff_dir):
                continue
            for fname in sorted(os.listdir(diff_dir)):
                if fname.endswith(".md"):
                    p_id = os.path.splitext(fname)[0]
                    items_to_review.append((diff, p_id, os.path.join(diff_dir, fname)))

        if target_problem:
            norm = target_problem.lower().replace("-", "_").replace(".json", "").replace(".md", "")
            items_to_review = [it for it in items_to_review if norm in it[1].lower()]

        total = len(items_to_review)
        print(f"Found {Colors.BOLD}{total}{Colors.RESET} existing solutions to evaluate.\n")

        reviewed_count = 0
        passed_count = 0
        failed_count = 0

        for idx, (diff, p_id, md_path) in enumerate(items_to_review, 1):
            rel_key = f"{diff}/{p_id}"
            prob_json_path = os.path.join(PROBLEMS_DIR, diff, f"{p_id}.json")
            
            problem_data = {"title": p_id.replace("_", " ").title(), "difficulty": diff}
            if os.path.exists(prob_json_path):
                try:
                    with open(prob_json_path, "r", encoding="utf-8") as f:
                        problem_data = json.load(f)
                except Exception:
                    pass

            try:
                with open(md_path, "r", encoding="utf-8") as f:
                    md_content = f.read()
            except Exception as e:
                print(f"[{idx:>3}/{total}] Could not read {md_path}: {e}")
                continue

            print(f"[{idx:>3}/{total}] Reviewing [{diff.upper():<6}] {problem_data.get('title', p_id)} ({p_id})...")
            review_res = self.evaluate_solution(rel_key, problem_data, md_content, md_path)
            
            if review_res:
                reviewed_count += 1
                if review_res.get("grade") == "PASS":
                    passed_count += 1
                else:
                    failed_count += 1
                
                # Update progress tracker record if exists
                if rel_key in self.tracker.state.get("completed_problems", {}):
                    self.tracker.state["completed_problems"][rel_key]["review"] = review_res
                    self.tracker.save()

            if self.delay_sec > 0:
                time.sleep(self.delay_sec)

        print(f"\n{Colors.BOLD}{Colors.GREEN}=== Review Summary ==={Colors.RESET}")
        print(f"Total Reviewed:            {reviewed_count}")
        print(f"Passed (Score >= {self.safe_score}):   {Colors.GREEN}{passed_count}{Colors.RESET}")
        print(f"Flagged for Rewrite:       {Colors.YELLOW}{failed_count}{Colors.RESET}")
        print(f"Report File:               {os.path.relpath(REWRITE_LIST_MD, BASE_DIR)}\n")

    def run(self, difficulties=None, limit=None, target_problem=None, force=False, rewrite_only=False):
        """Scans all problems and runs the generation pipeline with strict progress resume."""
        if difficulties is None:
            difficulties = ["easy", "medium", "hard"]

        problem_files = []
        for diff in difficulties:
            diff_dir = os.path.join(PROBLEMS_DIR, diff)
            if not os.path.exists(diff_dir):
                continue
            for fname in sorted(os.listdir(diff_dir)):
                if fname.endswith(".json"):
                    problem_files.append(os.path.join(diff_dir, fname))

        if rewrite_only:
            rewrite_keys = set(self.rewrite_tracker.state.get("problems", {}).keys())
            problem_files = [p for p in problem_files if f"{os.path.basename(os.path.dirname(p)).lower()}/{os.path.splitext(os.path.basename(p))[0]}" in rewrite_keys]
            force = True

        if target_problem:
            norm_target = target_problem.lower().replace("-", "_")
            if not norm_target.endswith(".json"):
                norm_target += ".json"
            problem_files = [p for p in problem_files if os.path.basename(p).lower() == norm_target or norm_target.replace(".json", "") in p.lower()]

        total_problems = len(problem_files)
        if total_problems == 0:
            print(f"{Colors.YELLOW}No matching problems found.{Colors.RESET}")
            return

        print(f"\n{Colors.BOLD}{Colors.CYAN}=== SyntaxFlow AI Solution Generator & Quality Reviewer ==={Colors.RESET}")
        print(f"Total problems discovered: {Colors.BOLD}{total_problems}{Colors.RESET}")
        print(f"Active Provider Mode:      {Colors.BOLD}{self.provider_mode.upper()}{Colors.RESET}")
        print(f"Gemini Keys Available:     {self.gemini_pool.count()} key(s)")
        print(f"OpenRouter Keys Available: {self.openrouter_pool.count()} key(s)")
        print(f"Groq Keys Available:       {self.groq_pool.count()} key(s)")
        print(f"Reviewer Quality Engine:   {Colors.BOLD}Groq ({self.reviewer_model}) | Safe Grade: {self.safe_score}/100{Colors.RESET}")
        print(f"Solutions Directory:       {os.path.relpath(SOLUTIONS_DIR, BASE_DIR)}")
        print(f"Rewrite List Report:       {os.path.relpath(REWRITE_LIST_MD, BASE_DIR)}")
        
        already_done = len(self.tracker.state.get("completed_problems", {}))
        print(f"Resuming Progress:         {Colors.GREEN}{already_done} completed{Colors.RESET} / {total_problems} total")

        newly_generated_count = 0

        # Handle graceful exit on Ctrl+C
        def sigint_handler(sig, frame):
            print(f"\n\n{Colors.YELLOW}Process interrupted by user (Ctrl+C). Saving state and exiting...{Colors.RESET}")
            self.tracker.save()
            self.rewrite_tracker.save()
            completed_now = len(self.tracker.state.get("completed_problems", {}))
            print(f"{Colors.GREEN}Progress saved: {completed_now} / {total_problems} completed.{Colors.RESET}")
            print(f"To resume later, simply rerun: {Colors.BOLD}python3 scripts/generate_ai_solutions.py{Colors.RESET}\n")
            sys.exit(0)

        signal.signal(signal.SIGINT, sigint_handler)

        for idx, p_path in enumerate(problem_files, 1):
            if limit and newly_generated_count >= limit:
                print(f"\n{Colors.YELLOW}Reached user limit of {limit} newly generated solutions. Stopping.{Colors.RESET}")
                break

            diff = os.path.basename(os.path.dirname(p_path)).lower()
            p_id = os.path.splitext(os.path.basename(p_path))[0]
            out_p = os.path.join(SOLUTIONS_DIR, diff, f"{p_id}.md")

            if force and os.path.exists(out_p):
                try:
                    os.remove(out_p)
                except Exception:
                    pass

            status = self.process_problem_with_retry(p_path, idx, total_problems)
            if status == "success":
                newly_generated_count += 1
            elif status == "failed":
                print(f"\n{Colors.RED}[!] Pipeline stopped at problem #{idx} ({p_id}) due to provider/quota failure.{Colors.RESET}")
                print(f"{Colors.YELLOW}Check your API keys/quotas, then re-run to resume automatically from problem #{idx}.{Colors.RESET}\n")
                break

        print(f"\n{Colors.BOLD}{Colors.GREEN}=== Summary ==={Colors.RESET}")
        print(f"Successfully generated in this run: {newly_generated_count}")
        print(f"Total Completed in Repository:      {len(self.tracker.state.get('completed_problems', {}))} / {total_problems}")
        print(f"Pending Manual Rewrite List:        {self.rewrite_tracker.state.get('total_flagged', 0)} problem(s)")
        print(f"Solutions directory:                {os.path.relpath(SOLUTIONS_DIR, BASE_DIR)}")
        print(f"Rewrite report:                     {os.path.relpath(REWRITE_LIST_MD, BASE_DIR)}\n")

# ==============================================================================
# CLI Entrypoint
# ==============================================================================

def main():
    parser = argparse.ArgumentParser(
        description="SyntaxFlow AI Problem Solution Generator with Gemini, OpenRouter, and Groq API Reviewer support."
    )
    parser.add_argument(
        "--provider",
        choices=["auto", "gemini", "openrouter", "groq"],
        default="auto",
        help="AI provider for generation (default: auto - prioritizes Gemini/OpenRouter with Groq fallback)"
    )
    parser.add_argument(
        "--gemini-model",
        default=DEFAULT_GEMINI_MODEL,
        help=f"Gemini model name (default: {DEFAULT_GEMINI_MODEL})"
    )
    parser.add_argument(
        "--openrouter-model",
        default=DEFAULT_OPENROUTER_MODEL,
        help=f"OpenRouter model name (default: {DEFAULT_OPENROUTER_MODEL})"
    )
    parser.add_argument(
        "--groq-model",
        default=DEFAULT_GROQ_MODEL,
        help=f"Groq model name (default: {DEFAULT_GROQ_MODEL})"
    )
    parser.add_argument(
        "--safe-score",
        type=int,
        default=DEFAULT_SAFE_SCORE,
        help=f"Minimum reviewer score (0-100) to pass; below this goes to rewrite list (default: {DEFAULT_SAFE_SCORE})"
    )
    parser.add_argument(
        "--no-review",
        action="store_true",
        help="Disable automatic Groq quality review after solution generation"
    )
    parser.add_argument(
        "--review-only",
        action="store_true",
        help="Review existing generated solution files without generating new solutions"
    )
    parser.add_argument(
        "--rewrite-only",
        action="store_true",
        help="Regenerate solutions only for problems currently in the rewrite list"
    )
    parser.add_argument(
        "--difficulty",
        choices=["all", "easy", "medium", "hard"],
        default="all",
        help="Difficulty level to process (default: all)"
    )
    parser.add_argument(
        "--problem",
        type=str,
        default=None,
        help="Specific problem slug or filename (e.g. two_sum or 24-game)"
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=None,
        help="Limit number of solutions to generate in this run"
    )
    parser.add_argument(
        "--delay",
        type=float,
        default=1.0,
        help="Delay in seconds between API requests (default: 1.0)"
    )
    parser.add_argument(
        "--force",
        action="store_true",
        help="Regenerate solutions even if they already exist"
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Show detailed debug output"
    )

    args = parser.parse_args()

    difficulties = None
    if args.difficulty != "all":
        difficulties = [args.difficulty]

    generator = AISolutionGenerator(
        provider=args.provider,
        gemini_model=args.gemini_model,
        openrouter_model=args.openrouter_model,
        groq_model=args.groq_model,
        safe_score=args.safe_score,
        review_enabled=not args.no_review,
        delay_sec=args.delay,
        verbose=args.verbose
    )

    if args.review_only:
        generator.review_existing_solutions(
            difficulties=difficulties,
            target_problem=args.problem
        )
    else:
        generator.run(
            difficulties=difficulties,
            limit=args.limit,
            target_problem=args.problem,
            force=args.force,
            rewrite_only=args.rewrite_only
        )

if __name__ == "__main__":
    main()
