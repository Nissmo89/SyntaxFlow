import os
import json
import requests
import time

def get_code_snippets(title_slug):
    url = "https://leetcode.com/graphql/"
    payload = {
        "query": """query questionData($titleSlug: String!) {
            question(titleSlug: $titleSlug) {
                codeSnippets {
                    lang
                    langSlug
                    code
                }
            }
        }""",
        "variables": {"titleSlug": title_slug}
    }
    headers = {
        "User-Agent": "Mozilla/5.0",
        "Content-Type": "application/json"
    }
    try:
        response = requests.post(url, json=payload, headers=headers)
        if response.status_code == 200:
            data = response.json()
            if "data" in data and "question" in data["data"] and data["data"]["question"]:
                snippets = data["data"]["question"].get("codeSnippets")
                if snippets:
                    result = {}
                    for snippet in snippets:
                        result[snippet["langSlug"]] = snippet["code"]
                    return result
    except Exception as e:
        print(f"Error fetching {title_slug}: {e}")
    return None

def main():
    problems_dir = "problems"
    
    # Check if problems directory exists
    if not os.path.exists(problems_dir):
        print(f"Error: Directory '{problems_dir}' not found.")
        return

    consecutive_failures = 0

    for root, _, files in os.walk(problems_dir):
        for file in files:
            if file.endswith(".json") and file != "problems.json":
                path = os.path.join(root, file)
                
                with open(path, "r", encoding="utf-8") as f:
                    try:
                        data = json.load(f)
                    except Exception as e:
                        print(f"Error reading {path}: {e}")
                        continue
                
                # Check if we already have func_sign
                if "func_sign" in data:
                    print(f"Skipping {file} (func_sign already exists)")
                    continue

                if "manifest" in data and "entry" in data["manifest"]:
                    title_slug = data["manifest"]["entry"].get("title")
                    if title_slug:
                        print(f"Fetching signatures for: {title_slug}")
                        snippets = get_code_snippets(title_slug)
                        if snippets:
                            # Add func_sign to data
                            data["func_sign"] = snippets
                            
                            with open(path, "w", encoding="utf-8") as f:
                                json.dump(data, f, indent=2)
                            
                            print(f" -> Successfully updated {title_slug}")
                            consecutive_failures = 0
                        else:
                            print(f" -> Failed to fetch snippets for {title_slug} (might be premium or invalid)")
                            consecutive_failures += 1
                            if consecutive_failures >= 15:
                                print("Too many consecutive failures. Aborting to prevent rate-limit penalties.")
                                return
                        
                        # Be gentle to Leetcode API
                        time.sleep(1.5)

if __name__ == "__main__":
    main()
