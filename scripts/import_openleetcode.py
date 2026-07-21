import os
import json
import yaml
import requests
import sys

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
OPENLEETCODE_TESTS = os.path.join(BASE_DIR, "openleetcode_temp", "tests")
PROBLEMS_DIR = os.path.join(BASE_DIR, "problems")
PROBLEMS_JSON = os.path.join(BASE_DIR, "problems.json")
GRAPHQL_URL = "https://leetcode.com/graphql"

LEETCODE_MAPPING = {
    "single_number": 136,
    "two_sum": 1,
    "best_time_buy_sell_stock": 121,
    "valid_parentheses": 20,
    "longest_common_prefix": 14,
    "binary_search": 704,
    "merge_sorted_arrays": 88,
    "subarray_sum_equals_k": 560,
    "maximum_subarray": 53,
    "rotate_matrix": 48,
    "linked_list_cycle": 141,
    "merge_two_lists": 21,
    "lru_cache": 146,
    "median_sorted_arrays": 4,
    "trapping_rain_water": 42,
    "word_search": 79,
    "n_queens": 51,
    "climbing_stairs": 70,
    "coin_change": 322,
    "longest_palindromic_substring": 5,
    "top_k_frequent_elements": 347,
    "count_set_bits": 191
}

def get_question_details(slug):
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
    resp = requests.post(
        GRAPHQL_URL,
        json={"query": query, "variables": {"titleSlug": slug}},
        headers={"Content-Type": "application/json"},
    )
    resp.raise_for_status()
    data = resp.json()
    if "errors" in data:
        raise RuntimeError(f"GraphQL errors: {data['errors']}")
    return data["data"]["question"]

def find_manifest_path(problem_id):
    # Walk through OPENLEETCODE_TESTS to find a folder matching {problem_id}.
    for root, dirs, files in os.walk(OPENLEETCODE_TESTS):
        for d in dirs:
            if d.startswith(f"{problem_id}."):
                manifest_path = os.path.join(root, d, "manifest.yaml")
                if os.path.exists(manifest_path):
                    return manifest_path
    return None

def format_input_case(inputs):
    # Format case inputs as user-friendly "name = value" lines
    lines = []
    for k, v in inputs.items():
        # Compact JSON serialization for values
        val_str = json.dumps(v)
        # Avoid double quoting simple strings if they contain nested structures, but keep it clean
        lines.append(f"{k} = {val_str}")
    return "\n".join(lines)

def build_method_schema(manifest):
    # Build method schema for backward compatibility / driver generators
    entry = manifest.get("entry", {})
    params_schema = entry.get("params", {})
    call = entry.get("call", {}).get("python3", "")
    
    # Try to extract method name from call, e.g., "Solution().twoSum({nums}, {target})"
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

def main():
    if not os.path.exists(PROBLEMS_JSON):
        print(f"Error: {PROBLEMS_JSON} not found!")
        sys.exit(1)
        
    with open(PROBLEMS_JSON, "r") as f:
        problems_list = json.load(f)
        
    for p in problems_list:
        p_id = p["id"]
        if p_id not in LEETCODE_MAPPING:
            print(f"Skipping non-LeetCode problem: {p_id}")
            continue
            
        lc_id = LEETCODE_MAPPING[p_id]
        manifest_path = find_manifest_path(lc_id)
        if not manifest_path:
            print(f"Warning: Manifest for LeetCode ID {lc_id} (problem {p_id}) not found!")
            continue
            
        print(f"Processing {p_id} (LeetCode ID {lc_id})...")
        
        with open(manifest_path, "r") as mf:
            manifest = yaml.safe_load(mf)
            
        # Get details from LeetCode GraphQL
        title_slug = manifest["entry"]["title"]
        try:
            details = get_question_details(title_slug)
        except Exception as e:
            print(f"Failed to fetch details for {title_slug}: {e}")
            details = None
            
        title = details["title"] if details else p.get("title", p_id.replace("_", " ").title())
        difficulty = details["difficulty"] if details else p.get("difficulty", "Medium")
        category = details["categoryTitle"] if details else p.get("category", "")
        tags = [t["name"] for t in details["topicTags"]] if details else p.get("topics", [])
        description = details["content"] if details else "No description available."
        hints = details["hints"] if details else []
        
        # Build testCases for UI
        test_cases = []
        for idx, tc in enumerate(manifest.get("tests", [])):
            # Only show first 5-8 test cases in the UI to keep it clean, but keep them all in manifest!
            if idx >= 8:
                break
            in_val = tc.get("in", {})
            out_val = tc.get("out")
            
            test_cases.append({
                "input": format_input_case(in_val),
                "output": json.dumps(out_val) if out_val is not None else ""
            })
            
        # Parse constraints from description if possible
        constraints = []
        if details and details["content"]:
            # Simple heuristic to extract constraints from HTML
            from bs4 import BeautifulSoup
            soup = BeautifulSoup(details["content"], "html.parser")
            constraints_header = None
            for p_tag in soup.find_all(["p", "strong", "em"]):
                if "constraints" in p_tag.text.lower():
                    constraints_header = p_tag
                    break
            if constraints_header:
                # Find following list items
                ul = constraints_header.find_next("ul")
                if ul:
                    constraints = [li.text.strip() for li in ul.find_all("li")]
                    
        # Construct final problem JSON
        problem_json = {
            "id": p_id,
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
        
        # Save to file
        dest_path = os.path.join(PROBLEMS_DIR, p["path"])
        os.makedirs(os.path.dirname(dest_path), exist_ok=True)
        with open(dest_path, "w") as df:
            json.dump(problem_json, df, indent=2)
            
        print(f"Successfully generated {dest_path}")

if __name__ == "__main__":
    main()
