import os
import subprocess

PROBLEMS_DIR = "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems"
TESTS_DIR = "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/core_Functionality/openleetcode/tests"
RUNNER_BIN = "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/build-headless/headless_test_runner"
ERRORS_DIR = "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/errors"

os.makedirs(ERRORS_DIR, exist_ok=True)

# Build a map of title_slug -> (prob_id, test_folder_path)
test_folders = {}
for range_folder in os.listdir(TESTS_DIR):
    range_path = os.path.join(TESTS_DIR, range_folder)
    if os.path.isdir(range_path):
        for prob_folder in os.listdir(range_path):
            prob_path = os.path.join(range_path, prob_folder)
            if os.path.isdir(prob_path):
                parts = prob_folder.split(". ", 1)
                if len(parts) == 2:
                    prob_id = int(parts[0])
                    slug = parts[1]
                    test_folders[slug] = (prob_id, prob_path)

def run_test(problem_json_path, language_id, sol_path, prob_id, prob_slug):
    if not os.path.exists(sol_path):
        return
        
    print(f"Running {prob_id}. {prob_slug} - {language_id} ...")
    
    cmd = [RUNNER_BIN, problem_json_path, language_id, sol_path]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60.0)
        
        # Check for failures
        if result.returncode != 0 or "SYSTEM_ERROR" in result.stdout or "COMPILATION_ERROR" in result.stdout or "Wrong Answer" in result.stdout or "Runtime Error" in result.stdout:
            error_file = os.path.join(ERRORS_DIR, f"{prob_id}_{prob_slug}_{language_id}_error.md")
            with open(error_file, "w") as f:
                f.write(f"# Error for Problem {prob_id} ({language_id})\n\n")
                f.write("## Stdout\n```\n")
                f.write(result.stdout)
                f.write("\n```\n")
                f.write("## Stderr\n```\n")
                f.write(result.stderr)
                f.write("\n```\n")
            print(f"  [FAILED] Logged to {error_file}")
        else:
            print(f"  [PASSED]")
            
    except subprocess.TimeoutExpired:
        error_file = os.path.join(ERRORS_DIR, f"{prob_id}_{prob_slug}_{language_id}_error.md")
        with open(error_file, "w") as f:
            f.write(f"# Timeout for Problem {prob_id} ({language_id})\n\n")
        print(f"  [TIMEOUT] Logged to {error_file}")

# Iterate through JSON files
for difficulty in ["easy", "medium", "hard"]:
    diff_dir = os.path.join(PROBLEMS_DIR, difficulty)
    if not os.path.exists(diff_dir):
        continue
        
    for json_file in os.listdir(diff_dir):
        if not json_file.endswith(".json"):
            continue
            
        json_path = os.path.join(diff_dir, json_file)
        prob_slug = json_file[:-5].replace("_", "-")
        
        if prob_slug not in test_folders:
            continue
            
        prob_id, test_folder = test_folders[prob_slug]
            
        # Run Python
        run_test(json_path, "python", os.path.join(test_folder, "sol.py"), prob_id, prob_slug)
        
        # Run JavaScript
        js_sol = os.path.join(test_folder, "sol.js")
        ts_sol = os.path.join(test_folder, "sol.ts")
        if os.path.exists(js_sol):
            run_test(json_path, "javascript", js_sol, prob_id, prob_slug)
        elif os.path.exists(ts_sol):
            import subprocess
            temp_js = os.path.join(test_folder, "temp_sol.js")
            subprocess.run(["./bin/micromamba", "run", "-n", "dev", "esbuild", ts_sol, "--outfile=" + temp_js], capture_output=True)
            if os.path.exists(temp_js):
                run_test(json_path, "javascript", temp_js, prob_id, prob_slug)
                os.remove(temp_js)
            
        # Run C++
        run_test(json_path, "cpp", os.path.join(test_folder, "sol.cpp"), prob_id, prob_slug)
