# Error for Problem 784 (python)

## Stdout
```
TEST_RESULT|0|Accepted|0
TEST_RESULT|1|Accepted|0
TEST_RESULT|2|Accepted|0
TEST_RESULT|3|Accepted|0
TEST_RESULT|4|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 206ne 388ne 570ne 752ne 934 1116, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 201, in letterCasePermutation
    solve(0, "")
  File "/src/user_code.py", line 190, in solve
    if idx == len(s):
TypeError: object of type 'int' has no len()

EXPECTED|
TEST_RESULT|5|Accepted|0
TEST_RESULT|6|Accepted|0
TEST_RESULT|7|Accepted|0
TEST_RESULT|8|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 206ne 388ne 570ne 752ne 934 1116, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 201, in letterCasePermutation
    solve(0, "")
  File "/src/user_code.py", line 190, in solve
    if idx == len(s):
TypeError: object of type 'int' has no len()

EXPECTED|
TEST_RESULT|9|Accepted|0
TEST_RESULT|10|Accepted|0
TEST_RESULT|11|Accepted|0
TEST_RESULT|12|Accepted|0
TEST_RESULT|13|Accepted|0
TEST_RESULT|14|Accepted|0
TEST_RESULT|15|Accepted|0
TEST_RESULT|16|Accepted|0
TEST_RESULT|17|Accepted|0
TEST_RESULT|18|Accepted|0
TEST_RESULT|19|Accepted|0
TEST_RESULT|20|Accepted|0
TEST_RESULT|21|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 206ne 388ne 570ne 752ne 934 1116, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 201, in letterCasePermutation
    solve(0, "")
  File "/src/user_code.py", line 190, in solve
    if idx == len(s):
TypeError: object of type 'int' has no len()

EXPECTED|
TEST_RESULT|22|Accepted|0
TEST_RESULT|23|Accepted|0
TEST_RESULT|24|Accepted|0
TEST_RESULT|25|Accepted|0
TEST_RESULT|26|Accepted|0
TEST_RESULT|27|Accepted|0
TEST_RESULT|28|Accepted|0
TEST_RESULT|29|Accepted|0
TEST_RESULT|30|Accepted|0

```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/letter_case_permutation.json"
Loaded 31 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/letter_case_permutation.json"
Loaded 31 test cases
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 0
PythonRunner: returning result

```
