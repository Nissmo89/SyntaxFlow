# Error for Problem 996 (python)

## Stdout
```
TEST_RESULT|0|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|1|Accepted|0
TEST_RESULT|2|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|3|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|4|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|5|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|6|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|7|Accepted|0
TEST_RESULT|8|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|9|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|10|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|11|Accepted|0
TEST_RESULT|12|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|13|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|14|Accepted|0
TEST_RESULT|15|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|16|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|17|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|18|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|19|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|20|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|21|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|22|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|23|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|24|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|25|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|26|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|27|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|28|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|29|Accepted|0
TEST_RESULT|30|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|31|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|32|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|33|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 227, in numSquarefulPerms
    return backtrack(0, [])
  File "/src/user_code.py", line 222, in backtrack
    num_squareful_perms += backtrack(nums_mask | (1 << idx), curr_nums)
  File "/src/user_code.py", line 219, in backtrack
    if (nums_mask & (1 << idx)) == 0 and is_squareful(curr_nums, num):
  File "/src/user_code.py", line 201, in is_squareful
    sqrt_n = math.isqrt(pair_sum)
AttributeError: module 'math' has no attribute 'isqrt'

EXPECTED|0
TEST_RESULT|34|Accepted|0

```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/hard/number_of_squareful_arrays.json"
Loaded 35 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/hard/number_of_squareful_arrays.json"
Loaded 35 test cases
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 0
PythonRunner: returning result

```
