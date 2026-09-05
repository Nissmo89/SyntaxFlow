# SyntaxFlow Harness & Sandbox Fixes

### ✅ 1. Missing Standard Typing Imports (`NameError`)
- **Example Failure:** `find_mode_in_binary_search_tree`
- **The Issue:** The headless runner injects `from typing import List, Optional, Any`, but it forgets to import `Dict`, `Set`, `Tuple`, and `Union`. If a solution's function signature contains `counter: Dict[int, int]`, the Python sandbox immediately crashes with `NameError: name 'Dict' is not defined`.
- **How to fix:** Add `Dict`, `Set`, `Tuple`, `Union`, `Callable` to the default imports in `code_runner.cpp`.

### ✅ 2. YAML-to-JSON Data Parsing Bugs (Base-60 Integers)
- **Example Failure:** `binary_watch`
- **The Issue:** The problem requires returning times like `"1:00"`. However, because the JSON was likely converted from YAML at some point during scraping, the YAML parser evaluated `"1:00"` as a base-60 integer. So, the JSON manifest literally expects `60`, `120`, and `240` instead of `"1:00"`, `"2:00"`, `"4:00"`.
- **How to fix:** We need a one-off script to clean up and stringify time-formatted data in the problem JSON files.

### ✅ 3. Missing `out` Fields in Test Cases
- **Example Failure:** `binary_tree_paths`
- **The Issue:** The JSON manifest for this problem literally has no `out` field for any of its test cases. The runner falls back to checking against `null` or an empty string, which obviously fails against the correct output.
- **How to fix:** We need to update the JSON manifests for problems missing their `out` fields, or ensure the scraper properly populates them.

### ✅ 4. Python Version Mismatch in Custom Oracles
- **Example Failure:** `counting_bits`
- **The Issue:** The test runner uses a custom checker (oracle) that calls `res.bit_count()`. However, `int.bit_count()` was introduced in Python 3.10, and our Wasmer sandbox is running Python 3.6.7. It crashes with `System Error: 'int' object has no attribute 'bit_count'`.
- **How to fix:** We either need to update the Wasmer Python package to 3.10+, or rewrite the oracle checkers in the JSON files to be backward compatible (e.g., using `bin(x).count('1')`).

### ✅ 5. Infinite Loops / Cycle Detection in `to_json`
- **Example Failure:** Problems returning Trees or Graphs.
- **The Issue:** Our custom `to_json` serialization function in `code_runner.cpp` occasionally trips a `ValueError("Cycle detected while encoding to json")` when trying to parse `ListNode` or `TreeNode` outputs that aren't cleanly flattened by `deserialize_val`.
- **How to fix:** Update the `_encode` function in `code_runner.cpp` to explicitly handle or flatten `ListNode` and `TreeNode` classes instead of dynamically traversing their `__dict__`.
