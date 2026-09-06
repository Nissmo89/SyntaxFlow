#!/usr/bin/env python3
"""
SyntaxFlow Batch Test Runner
============================

This script runs the SyntaxFlow `headless_test_runner` over a batch of problem JSON files
and their corresponding extracted code solutions, collecting and logging all test failures.

Features:
- Tests all solutions for a specific language (python, cpp, js) across the `problems/` directory.
- Maps solutions from the `headless_solutions/` directory.
- Captures `COMPILATION_ERROR`, `SYSTEM_ERROR`, and Wrong Answer/Runtime Errors automatically.
- Enforces a 30-second timeout per problem to prevent infinite hangs.
- Outputs detailed crash logs, expected vs actual outputs, and tracebacks to `test_failures_<lang>.log`.
- (New) Supports filtering by a text file containing specific problem names to run (e.g., failed_problems.txt).

Usage Examples:
---------------
1. Test all Python solutions:
   python3 batch_test_runner.py python

2. Test all C++ solutions:
   python3 batch_test_runner.py cpp

3. Test ONLY specific problems listed in a text file (one problem name per line):
   python3 batch_test_runner.py python --filter-file failed_problems.txt

4. Show help menu:
   python3 batch_test_runner.py --help
"""

import os
import sys
import json
import argparse
import subprocess
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(
        description="SyntaxFlow Batch Test Runner for headless environments.",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument(
        "language",
        type=str,
        nargs="?",
        help="The language to test (python, js, cpp)."
    )
    parser.add_argument(
        "--filter-file",
        "-f",
        type=str,
        help="Path to a text file containing specific problem names to test (one per line).",
        default=None
    )

    args = parser.parse_args()

    if args.language:
        lang = args.language.lower()
    else:
        lang = input("Which language to test? (python/js/cpp): ").strip().lower()

    ext_map = {'python': 'py', 'js': 'js', 'javascript': 'js', 'cpp': 'cpp', 'c++': 'cpp'}
    if lang not in ext_map:
        print(f"Unknown language: {lang}")
        sys.exit(1)
        
    ext = ext_map[lang]
    # The runner expects language id, e.g., 'python', 'javascript', 'cpp'
    runner_lang_id = 'javascript' if lang == 'js' else lang
    runner_lang_id = 'cpp' if lang == 'c++' else runner_lang_id
    
    runner_exec = Path("build-headless/headless_test_runner")
    if not runner_exec.exists():
        print(f"Error: Executable {runner_exec} not found. Build it first.")
        sys.exit(1)

    problems_dir = Path("problems")
    solutions_dir = Path("headless_solutions")
    
    # Load filter file if provided
    allowed_problems = None
    if args.filter_file:
        filter_path = Path(args.filter_file)
        if not filter_path.exists():
            print(f"Error: Filter file {filter_path} not found.")
            sys.exit(1)
        with open(filter_path, "r", encoding="utf-8") as ff:
            allowed_problems = set(line.strip() for line in ff if line.strip())
        print(f"Loaded {len(allowed_problems)} problems from {filter_path} to test.")
    
    log_file = f"test_failures_{lang}.log"
    
    problems_tested = 0
    problems_failed = 0
    
    with open(log_file, "w", encoding="utf-8") as f_log:
        f_log.write(f"=== Batch Test Run for {lang} ===\n\n")
        
        # Iterate over all problem json files
        for problem_json in sorted(problems_dir.rglob("*.json")):
            problem_name = problem_json.stem
            
            # Skip if filter file is provided and problem is not in it
            if allowed_problems is not None and problem_name not in allowed_problems:
                continue
                
            solution_file = solutions_dir / problem_name / f"solution.{ext}"
            if not solution_file.exists():
                if allowed_problems is not None:
                    print(f"Warning: Solution file for '{problem_name}' not found, skipping.")
                continue
                
            problems_tested += 1
            print(f"[{problems_tested}] Testing {problem_name}... ", end="", flush=True)
            
            # Run the headless test runner
            cmd = [str(runner_exec), str(problem_json), runner_lang_id, str(solution_file)]
            try:
                result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
                output = result.stdout + result.stderr
            except subprocess.TimeoutExpired:
                output = "SYSTEM_ERROR|Timeout expired after 30 seconds"
                result = subprocess.CompletedProcess(args=cmd, returncode=1, stdout=output, stderr="")
            
            # Analyze output for failures
            has_failure = False
            failure_details = []
            
            lines = output.split('\n')
            for line in lines:
                if line.startswith("COMPILATION_ERROR|") or line.startswith("SYSTEM_ERROR|"):
                    has_failure = True
                    failure_details.append(line)
                elif line.startswith("TEST_RESULT|"):
                    parts = line.split('|')
                    if len(parts) >= 3 and parts[2] != "Accepted":
                        has_failure = True
                        failure_details.append(line)
                elif has_failure and line.strip(): # Capture accompanying output/expected lines
                    if line.startswith("OUTPUT|") or line.startswith("EXPECTED|") or "Traceback" in line or "Error" in line:
                        failure_details.append(line)
                        
            # Fuzzy match floats for Wrong Answer
            if has_failure:
                new_details = []
                temp_has_failure = False
                i = 0
                while i < len(failure_details):
                    if failure_details[i].startswith("TEST_RESULT|") and "Wrong Answer" in failure_details[i]:
                        if i + 2 < len(failure_details) and failure_details[i+1].startswith("OUTPUT|") and failure_details[i+2].startswith("EXPECTED|"):
                            try:
                                import ast, math
                                def fuzzy(a, b):
                                    if isinstance(a, float) or isinstance(b, float): return math.isclose(float(a), float(b), rel_tol=1e-5, abs_tol=1e-5)
                                    if type(a) == list and type(b) == list and len(a) == len(b): return all(fuzzy(x, y) for x, y in zip(a, b))
                                    return a == b
                                out_val = ast.literal_eval(failure_details[i+1][7:].strip())
                                exp_val = ast.literal_eval(failure_details[i+2][9:].strip())
                                if fuzzy(out_val, exp_val):
                                    i += 3
                                    continue
                            except Exception:
                                pass
                    temp_has_failure = True
                    new_details.append(failure_details[i])
                    i += 1
                has_failure = temp_has_failure
                failure_details = new_details

            if result.returncode != 0 and not has_failure:
                has_failure = True
                failure_details.append(f"Non-zero exit code: {result.returncode}")
                failure_details.append(output[:500]) # Snippet of raw output
                
            if has_failure:
                print("FAILED ❌")
                problems_failed += 1
                
                f_log.write(f"--- FAILED: {problem_name} ---\n")
                f_log.write(f"Problem JSON: {problem_json}\n")
                f_log.write(f"Solution: {solution_file}\n")
                for detail in failure_details:
                    f_log.write(f"{detail}\n")
                
                # Try to capture the raw error text if it was a traceback
                raw_errors = [l for l in lines if not l.startswith(("TEST_RESULT", "OUTPUT", "EXPECTED", "COMPILATION_ERROR", "SYSTEM_ERROR")) and l.strip()]
                if raw_errors:
                    f_log.write("Raw output snippet:\n")
                    f_log.write("\n".join(raw_errors[:20]) + "\n")
                f_log.write("\n")
                f_log.flush()
            else:
                print("PASSED ✅")
                
    print(f"\nFinished testing {problems_tested} {lang} solutions.")
    print(f"Total failures: {problems_failed}")
    print(f"Log written to {log_file}")

if __name__ == "__main__":
    main()
