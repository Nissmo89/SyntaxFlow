#!/usr/bin/env python3
"""
SyntaxFlow - AI Problem Solution Generator
=========================================
Gathers problem metadata, test cases, function signatures, and live LeetCode
GraphQL raw editorial/community discussions, then prompts Google Gemini or Groq
to generate world-class, clean, multi-language editorial markdown solutions.

Features:
- Progress tracking with resume capability (re-starts where you left off).
- Dual API support: Google Gemini (gemini-2.0-flash / gemini-1.5-flash) and Groq (llama-3.3-70b).
- Automatic failover between Gemini and Groq on rate limits (HTTP 429).
- Key rotation for multiple comma-separated API keys.
- Outputs clean .md files to solutions/{difficulty}/{problem_id}.md.
- Standalone: Zero mandatory external dependencies (standard Python library).
"""

import os
import sys
import json
import time
import re
import html
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
ENV_FILE = os.path.join(BASE_DIR, ".env")

DEFAULT_GEMINI_MODEL = "gemini-2.0-flash"
DEFAULT_GROQ_MODEL = "llama-3.3-70b-versatile"
LEETCODE_GRAPHQL_URL = "https://leetcode.com/graphql"

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
        # Clean HTML inside pre
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
            "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36",
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
                        "content": sol.get("content", "")[:4000] # Cap size for context
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
                            "content": cnt[:3000] # Cap size for prompt context
                        })

                return {"official": official, "community": community}
    except Exception:
        # Graceful fallback: return empty dict if network or LeetCode is unreachable
        pass

    return {"official": None, "community": []}

# ==============================================================================
# AI Provider Clients: Gemini & Groq
# ==============================================================================

SYSTEM_PROMPT = """You are an elite competitive programmer, algorithm engineer, and master computer science educator.
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
   Include implementations in the following languages:
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

class GeminiClient:
    """Handles communication with Google Gemini API."""
    def __init__(self, key_pool, model=DEFAULT_GEMINI_MODEL):
        self.key_pool = key_pool
        self.model = model

    def generate(self, prompt, system_instruction=SYSTEM_PROMPT, timeout_sec=60):
        key = self.key_pool.get_current()
        if not key:
            raise ValueError("No Gemini API key provided")

        url = f"https://generativelanguage.googleapis.com/v1beta/models/{self.model}:generateContent?key={key}"
        
        payload = {
            "contents": [
                {
                    "role": "user",
                    "parts": [{"text": prompt}]
                }
            ],
            "systemInstruction": {
                "role": "system",
                "parts": [{"text": system_instruction}]
            },
            "generationConfig": {
                "temperature": 0.2,
                "maxOutputTokens": 8192
            }
        }

        req = urllib.request.Request(
            url,
            data=json.dumps(payload).encode("utf-8"),
            headers={"Content-Type": "application/json"}
        )

        try:
            with urllib.request.urlopen(req, timeout=timeout_sec) as resp:
                data = json.loads(resp.read().decode("utf-8"))
                candidates = data.get("candidates", [])
                if candidates:
                    parts = candidates[0].get("content", {}).get("parts", [])
                    if parts:
                        return parts[0].get("text", "")
                raise ValueError(f"Empty Gemini response: {json.dumps(data)[:200]}")
        except urllib.error.HTTPError as e:
            err_body = e.read().decode("utf-8", errors="ignore")
            if e.code == 429:
                self.key_pool.rotate()
                raise RuntimeError(f"Gemini Rate Limit (429): {err_body[:150]}")
            raise RuntimeError(f"Gemini HTTP {e.code}: {err_body[:200]}")
        except Exception as e:
            raise RuntimeError(f"Gemini Request failed: {e}")


class GroqClient:
    """Handles communication with Groq API (OpenAI compatible endpoint)."""
    def __init__(self, key_pool, model=DEFAULT_GROQ_MODEL):
        self.key_pool = key_pool
        self.model = model

    def generate(self, prompt, system_instruction=SYSTEM_PROMPT, timeout_sec=60):
        key = self.key_pool.get_current()
        if not key:
            raise ValueError("No Groq API key provided")

        url = "https://api.groq.com/openai/v1/chat/completions"
        
        payload = {
            "model": self.model,
            "messages": [
                {"role": "system", "content": system_instruction},
                {"role": "user", "content": prompt}
            ],
            "temperature": 0.2,
            "max_tokens": 8000
        }

        req = urllib.request.Request(
            url,
            data=json.dumps(payload).encode("utf-8"),
            headers={
                "Content-Type": "application/json",
                "Authorization": f"Bearer {key}"
            }
        )

        try:
            with urllib.request.urlopen(req, timeout=timeout_sec) as resp:
                data = json.loads(resp.read().decode("utf-8"))
                choices = data.get("choices", [])
                if choices:
                    return choices[0].get("message", {}).get("content", "")
                raise ValueError(f"Empty Groq response: {json.dumps(data)[:200]}")
        except urllib.error.HTTPError as e:
            err_body = e.read().decode("utf-8", errors="ignore")
            if e.code == 429:
                self.key_pool.rotate()
                raise RuntimeError(f"Groq Rate Limit (429): {err_body[:150]}")
            raise RuntimeError(f"Groq HTTP {e.code}: {err_body[:200]}")
        except Exception as e:
            raise RuntimeError(f"Groq Request failed: {e}")

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
        
        # Write atomically using a temp file
        temp_path = self.filepath + ".tmp"
        with open(temp_path, "w", encoding="utf-8") as f:
            json.dump(self.state, f, indent=2)
        os.replace(temp_path, self.filepath)

    def is_completed(self, rel_key, md_filepath):
        """Returns True if marked in state and output .md file exists with valid content."""
        if rel_key in self.state.get("completed_problems", {}):
            if os.path.exists(md_filepath) and os.path.getsize(md_filepath) > 100:
                return True
        # Check if output .md already exists with substantial content
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

    # Reference raw LeetCode content (editorials / top community solutions)
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
# Main Generation Pipeline
# ==============================================================================

class AISolutionGenerator:
    def __init__(self, provider="auto", gemini_model=DEFAULT_GEMINI_MODEL, groq_model=DEFAULT_GROQ_MODEL, delay_sec=1.0, verbose=False):
        self.provider_mode = provider
        self.gemini_model = gemini_model
        self.groq_model = groq_model
        self.delay_sec = delay_sec
        self.verbose = verbose

        # API Keys
        gemini_keys = os.environ.get("GEMINI_API_KEY", "") or os.environ.get("GOOGLE_API_KEY", "")
        groq_keys = os.environ.get("GROQ_API_KEY", "")

        self.gemini_pool = KeyPool(gemini_keys)
        self.groq_pool = KeyPool(groq_keys)

        self.gemini_client = GeminiClient(self.gemini_pool, model=gemini_model) if self.gemini_pool.has_keys() else None
        self.groq_client = GroqClient(self.groq_pool, model=groq_model) if self.groq_pool.has_keys() else None

        self.tracker = ProgressTracker()

    def generate_with_failover(self, prompt):
        """Attempts generation with primary provider, falling back to secondary if needed."""
        providers = []
        if self.provider_mode == "gemini":
            if not self.gemini_client:
                raise ValueError("GEMINI_API_KEY not set in environment or .env file")
            providers = [("Gemini", self.gemini_client)]
        elif self.provider_mode == "groq":
            if not self.groq_client:
                raise ValueError("GROQ_API_KEY not set in environment or .env file")
            providers = [("Groq", self.groq_client)]
        else: # auto
            if self.gemini_client:
                providers.append(("Gemini", self.gemini_client))
            if self.groq_client:
                providers.append(("Groq", self.groq_client))

        if not providers:
            raise ValueError("No API keys found. Please set GEMINI_API_KEY or GROQ_API_KEY in .env or environment variables.")

        last_error = None
        for name, client in providers:
            try:
                if self.verbose:
                    print(f"{Colors.DIM}  -> Querying {name} ({client.model})...{Colors.RESET}")
                t0 = time.time()
                result = client.generate(prompt)
                elapsed = time.time() - t0
                return result, name, client.model, elapsed
            except Exception as e:
                last_error = e
                print(f"{Colors.YELLOW}  [!] {name} failed: {e}. Attempting failover...{Colors.RESET}")
                time.sleep(1)

        raise RuntimeError(f"All AI providers failed. Last error: {last_error}")

    def process_problem(self, problem_filepath, index, total):
        """Processes a single problem JSON file and writes the generated .md solution."""
        difficulty = os.path.basename(os.path.dirname(problem_filepath)).lower()
        filename = os.path.basename(problem_filepath)
        problem_id = os.path.splitext(filename)[0]
        rel_key = f"{difficulty}/{problem_id}"

        # Target output markdown file
        out_dir = os.path.join(SOLUTIONS_DIR, difficulty)
        os.makedirs(out_dir, exist_ok=True)
        out_filepath = os.path.join(out_dir, f"{problem_id}.md")

        # Check if already completed
        if self.tracker.is_completed(rel_key, out_filepath):
            print(f"{Colors.DIM}[{index:>3}/{total}] [SKIP] {difficulty.upper():<6} {problem_id} (Already generated){Colors.RESET}")
            return True

        # Load problem data
        try:
            with open(problem_filepath, "r", encoding="utf-8") as f:
                problem_data = json.load(f)
        except Exception as e:
            print(f"{Colors.RED}[{index:>3}/{total}] [ERROR] Could not read {problem_filepath}: {e}{Colors.RESET}")
            self.tracker.mark_failed(rel_key, str(e))
            return False

        title = problem_data.get("title", problem_id)
        slug = problem_data.get("manifest", {}).get("entry", {}).get("title") or problem_id.replace("_", "-")

        print(f"{Colors.CYAN}[{index:>3}/{total}] [{difficulty.upper():<6}] {title} ({problem_id})...{Colors.RESET}")

        # Step 1: Gather raw LeetCode data (editorial + community solutions)
        if self.verbose:
            print(f"{Colors.DIM}  -> Fetching LeetCode GraphQL data for slug: {slug}...{Colors.RESET}")
        raw_leetcode = fetch_leetcode_raw_solutions(slug)

        # Step 2: Build rich prompt
        prompt = build_user_prompt(problem_data, raw_leetcode)

        # Step 3: Query AI with retry and failover
        try:
            markdown_content, prov_name, model_name, elapsed = self.generate_with_failover(prompt)
            clean_md = sanitize_ai_output(markdown_content)

            if len(clean_md) < 150:
                raise ValueError("Generated markdown is too short or empty")

            # Step 4: Write output file
            with open(out_filepath, "w", encoding="utf-8") as f:
                f.write(clean_md + "\n")

            # Step 5: Mark completed in progress tracker
            word_count = len(clean_md.split())
            self.tracker.mark_completed(rel_key, {
                "title": title,
                "provider": prov_name,
                "model": model_name,
                "file_path": os.path.relpath(out_filepath, BASE_DIR),
                "words": word_count,
                "elapsed_sec": round(elapsed, 2)
            })

            print(f"{Colors.GREEN}  [✓] Generated -> {os.path.relpath(out_filepath, BASE_DIR)} ({prov_name} {model_name} | {elapsed:.1f}s | {word_count} words){Colors.RESET}")
            
            if self.delay_sec > 0:
                time.sleep(self.delay_sec)
            return True

        except Exception as e:
            print(f"{Colors.RED}  [✗] Failed to generate {rel_key}: {e}{Colors.RESET}")
            self.tracker.mark_failed(rel_key, str(e))
            return False

    def run(self, difficulties=None, limit=None, target_problem=None, force=False):
        """Scans all problems and runs the generation pipeline with progress resume."""
        if difficulties is None:
            difficulties = ["easy", "medium", "hard"]

        # Collect problem files
        problem_files = []
        for diff in difficulties:
            diff_dir = os.path.join(PROBLEMS_DIR, diff)
            if not os.path.exists(diff_dir):
                continue
            for fname in sorted(os.listdir(diff_dir)):
                if fname.endswith(".json"):
                    problem_files.append(os.path.join(diff_dir, fname))

        if target_problem:
            # Filter for specific problem
            norm_target = target_problem.lower().replace("-", "_")
            if not norm_target.endswith(".json"):
                norm_target += ".json"
            problem_files = [p for p in problem_files if os.path.basename(p).lower() == norm_target or norm_target.replace(".json", "") in p.lower()]

        total_problems = len(problem_files)
        if total_problems == 0:
            print(f"{Colors.YELLOW}No matching problems found in {PROBLEMS_DIR}.{Colors.RESET}")
            return

        print(f"\n{Colors.BOLD}{Colors.CYAN}=== SyntaxFlow AI Solution Generator ==={Colors.RESET}")
        print(f"Total problems discovered: {Colors.BOLD}{total_problems}{Colors.RESET}")
        print(f"Active Provider Mode:      {Colors.BOLD}{self.provider_mode.upper()}{Colors.RESET}")
        print(f"Gemini Keys Available:     {self.gemini_pool.count()} key(s)")
        print(f"Groq Keys Available:       {self.groq_pool.count()} key(s)")
        print(f"Solutions Directory:       {os.path.relpath(SOLUTIONS_DIR, BASE_DIR)}")
        print(f"Progress Tracker File:     {os.path.relpath(PROGRESS_FILE, BASE_DIR)}")
        
        already_done = len(self.tracker.state.get("completed_problems", {}))
        print(f"Resuming Progress:         {Colors.GREEN}{already_done} completed{Colors.RESET} / {total_problems} total\n")

        processed_count = 0
        success_count = 0

        for idx, p_path in enumerate(problem_files, 1):
            if limit and processed_count >= limit:
                print(f"\n{Colors.YELLOW}Reached user limit of {limit} problems. Stopping.{Colors.RESET}")
                break

            diff = os.path.basename(os.path.dirname(p_path)).lower()
            p_id = os.path.splitext(os.path.basename(p_path))[0]
            rel_k = f"{diff}/{p_id}"
            out_p = os.path.join(SOLUTIONS_DIR, diff, f"{p_id}.md")

            if force and os.path.exists(out_p):
                try:
                    os.remove(out_p)
                except Exception:
                    pass

            ok = self.process_problem(p_path, idx, total_problems)
            processed_count += 1
            if ok:
                success_count += 1

        print(f"\n{Colors.BOLD}{Colors.GREEN}=== Batch Run Complete ==={Colors.RESET}")
        print(f"Processed: {processed_count} | Success: {success_count} | Total Stored: {len(self.tracker.state.get('completed_problems', {}))}")
        print(f"Solutions saved in: {os.path.relpath(SOLUTIONS_DIR, BASE_DIR)}\n")

# ==============================================================================
# CLI Entrypoint
# ==============================================================================

def main():
    parser = argparse.ArgumentParser(
        description="SyntaxFlow AI Problem Solution Generator with Gemini and Groq API support."
    )
    parser.add_argument(
        "--provider",
        choices=["auto", "gemini", "groq"],
        default="auto",
        help="AI provider to use (default: auto - uses Gemini with fallback to Groq)"
    )
    parser.add_argument(
        "--gemini-model",
        default=DEFAULT_GEMINI_MODEL,
        help=f"Gemini model name (default: {DEFAULT_GEMINI_MODEL})"
    )
    parser.add_argument(
        "--groq-model",
        default=DEFAULT_GROQ_MODEL,
        help=f"Groq model name (default: {DEFAULT_GROQ_MODEL})"
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
        help="Limit number of problems to generate in this run"
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
        groq_model=args.groq_model,
        delay_sec=args.delay,
        verbose=args.verbose
    )

    generator.run(
        difficulties=difficulties,
        limit=args.limit,
        target_problem=args.problem,
        force=args.force
    )

if __name__ == "__main__":
    main()
