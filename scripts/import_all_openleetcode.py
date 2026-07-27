import os
import json
import yaml
import requests
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from bs4 import BeautifulSoup

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
OPENLEETCODE_TESTS = os.path.join(BASE_DIR, "openleetcode_temp", "tests")
PROBLEMS_DIR = os.path.join(BASE_DIR, "problems")
PROBLEMS_JSON = os.path.join(BASE_DIR, "problems.json")
GRAPHQL_URL = "https://leetcode.com/graphql"

def get_question_details_with_retry(slug, retries=3, delay=1.0):
    query = """
    query questionDetail($titleSlug: String!) {
      question(titleSlug: $titleSlug) {
        content
        questionId
        title
        difficulty
        categoryTitle
        topicTags {
          name
        }
        hints
      }
    }
    """
    for attempt in range(retries):
        try:
            resp = requests.post(
                GRAPHQL_URL,
                json={"query": query, "variables": {"titleSlug": slug}},
                headers={"Content-Type": "application/json"},
                timeout=10
            )
            resp.raise_for_status()
            data = resp.json()
            if "errors" in data:
                if any("not found" in err.get("message", "").lower() for err in data["errors"]):
                    return None
                raise RuntimeError(f"GraphQL errors: {data['errors']}")
            return data["data"]["question"]
        except Exception as e:
            if attempt == retries - 1:
                print(f"Failed to fetch details for {slug} after {retries} attempts: {e}")
                return None
            time.sleep(delay * (2 ** attempt))

def format_input_case(inputs):
    lines = []
    for k, v in inputs.items():
        val_str = json.dumps(v)
        lines.append(f"{k} = {val_str}")
    return "\n".join(lines)

def build_method_schema(manifest):
    entry = manifest.get("entry", {})
    params_schema = entry.get("params", {})
    call = entry.get("call", {}).get("python3", "")
    
    method_name = "solve"
    if "Solution()." in call:
        parts = call.split("Solution().")[1].split("(")
        if parts:
            method_name = parts[0]
            
    params = []
    for name, schema in params_schema.items():
        params.append({
            "name": name,
            "type": schema.get("type", "int")
        })
        
    return {
        "name": method_name,
        "returnType": "auto",
        "params": params
    }

def process_manifest(manifest_path, existing_problems):
    try:
        with open(manifest_path, "r") as mf:
            manifest = yaml.safe_load(mf)
    except Exception as e:
        print(f"Failed to parse manifest {manifest_path}: {e}")
        return None

    entry = manifest.get("entry", {})
    title_slug = entry.get("title")
    if not title_slug:
        return None

    # Determine unique problem_id
    problem_id = title_slug.replace("-", "_")

    # Fetch details from LeetCode GraphQL
    details = get_question_details_with_retry(title_slug)
    
    title = details["title"] if details else title_slug.replace("-", " ").title()
    difficulty = details["difficulty"] if details else "Medium"
    category = details["categoryTitle"] if details else ""
    tags = [t["name"] for t in details["topicTags"]] if details else []
    description = details["content"] if details else "No description available."
    hints = details["hints"] if details else []

    # Build test cases for UI preview (max 8)
    test_cases = []
    for idx, tc in enumerate(manifest.get("tests", [])):
        in_val = tc.get("in", {})
        out_val = tc.get("out")
        
        test_cases.append({
            "input": format_input_case(in_val),
            "output": json.dumps(out_val) if out_val is not None else ""
        })

    # Extract constraints
    constraints = []
    if details and details["content"]:
        try:
            soup = BeautifulSoup(details["content"], "html.parser")
            constraints_header = None
            for p_tag in soup.find_all(["p", "strong", "em"]):
                if "constraints" in p_tag.text.lower():
                    constraints_header = p_tag
                    break
            if constraints_header:
                ul = constraints_header.find_next("ul")
                if ul:
                    constraints = [li.text.strip() for li in ul.find_all("li")]
        except Exception:
            pass

    # Save details
    problem_json = {
        "id": problem_id,
        "title": title,
        "difficulty": difficulty,
        "category": category,
        "tags": tags,
        "description": description,
        "task": f"Solve the '{title}' problem on LeetCode.",
        "inputFormat": "Passed directly as parameters.",
        "outputFormat": "Returned value from the solution method.",
        "constraints": constraints,
        "hints": hints,
        "method": build_method_schema(manifest),
        "manifest": manifest,
        "testCases": test_cases
    }

    # Output path
    difficulty_dir = difficulty.lower()
    dest_relative = f"{difficulty_dir}/{problem_id}.json"
    dest_path = os.path.join(PROBLEMS_DIR, dest_relative)
    
    os.makedirs(os.path.dirname(dest_path), exist_ok=True)
    with open(dest_path, "w") as df:
        json.dump(problem_json, df, indent=2)

    # Preserve status if it existed
    status = {"solved": False, "starred": False}
    if problem_id in existing_problems:
        status = existing_problems[problem_id].get("status", status)

    return {
        "id": problem_id,
        "title": title,
        "difficulty": difficulty,
        "path": dest_relative,
        "status": status,
        "topics": tags
    }

def main():
    if not os.path.exists(OPENLEETCODE_TESTS):
        print(f"Error: {OPENLEETCODE_TESTS} not found!")
        sys.exit(1)

    # Load existing problems to preserve status & non-LeetCode problems
    existing_problems = {}
    non_leetcode_problems = []
    if os.path.exists(PROBLEMS_JSON):
        try:
            with open(PROBLEMS_JSON, "r") as f:
                problems_list = json.load(f)
                for p in problems_list:
                    existing_problems[p["id"]] = p
                    # Simple heuristic: if it has no path or path doesn't exist, it's custom / non-leetcode
                    # Or we check if the id is hello_world, etc.
                    if p["id"] in ["hello_world", "sum_of_digits", "reverse_number", "palindrome_string"]:
                        non_leetcode_problems.append(p)
        except Exception as e:
            print(f"Failed to read existing problems.json: {e}")

    # Gather all manifest.yaml paths
    manifest_paths = []
    for root, dirs, files in os.walk(OPENLEETCODE_TESTS):
        for f in files:
            if f == "manifest.yaml":
                manifest_paths.append(os.path.join(root, f))

    print(f"Found {len(manifest_paths)} manifests to import. Executing parallel import...")

    new_problems = []
    # Use ThreadPoolExecutor for highly concurrent network requests
    with ThreadPoolExecutor(max_workers=20) as executor:
        futures = {executor.submit(process_manifest, path, existing_problems): path for path in manifest_paths}
        
        count = 0
        for future in as_completed(futures):
            res = future.result()
            count += 1
            if res:
                new_problems.append(res)
                if count % 50 == 0:
                    print(f"Imported {count}/{len(manifest_paths)} problems...")

    # Sort new problems by ID to make it clean
    new_problems.sort(key=lambda x: x["title"])

    # Combine non-leetcode (first) and imported leetcode problems
    final_list = non_leetcode_problems + new_problems

    # Save to problems.json
    with open(PROBLEMS_JSON, "w") as f:
        json.dump(final_list, f, indent=2)

    print(f"Done! Successfully generated {len(new_problems)} LeetCode problems.")
    print(f"Total problems in database: {len(final_list)}")

if __name__ == "__main__":
    main()
