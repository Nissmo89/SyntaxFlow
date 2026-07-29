import os
import json

def has_gen(tc_in):
    if isinstance(tc_in, dict):
        if "gen" in tc_in:
            return True
        for k, v in tc_in.items():
            if has_gen(v):
                return True
    elif isinstance(tc_in, list):
        for item in tc_in:
            if has_gen(item):
                return True
    return False

problems_dir = "problems"
count = 0
for root, _, files in os.walk(problems_dir):
    for f in files:
        if f.endswith(".json") and f != "problems.json":
            path = os.path.join(root, f)
            with open(path, "r") as fp:
                try:
                    data = json.load(fp)
                except:
                    continue
            
            modified = False
            if "manifest" in data and "tests" in data["manifest"]:
                tests = data["manifest"]["tests"]
                new_tests = [tc for tc in tests if not has_gen(tc.get("in", {}))]
                if len(new_tests) != len(tests):
                    data["manifest"]["tests"] = new_tests
                    modified = True
            
            if "testCases" in data:
                # also rebuild testCases to sync or just remove those that contain 'gen":'
                # It's safer to just rebuild testCases exactly like import_openleetcode.py
                new_test_cases = []
                for tc in data.get("manifest", {}).get("tests", []):
                    # Format case inputs as user-friendly "name = value" lines
                    lines = []
                    for k, v in tc.get("in", {}).items():
                        lines.append(f"{k} = {json.dumps(v)}")
                    out_val = tc.get("out")
                    new_test_cases.append({
                        "input": "\n".join(lines),
                        "output": json.dumps(out_val) if out_val is not None else ""
                    })
                if len(new_test_cases) != len(data["testCases"]) or modified:
                    data["testCases"] = new_test_cases
                    modified = True
                    
            if modified:
                with open(path, "w") as fp:
                    json.dump(data, fp, indent=2)
                count += 1

print(f"Fixed {count} files.")
