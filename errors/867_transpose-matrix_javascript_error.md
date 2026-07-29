# Error for Problem 867 (javascript)

## Stdout
```
TEST_RESULT|0|Runtime Error|0
OUTPUT|TypeEuser_code.js:10929er_code.js:11021er_code.js:11113)
    at <eval> (<input>)
    at run_all_tests (/src/user_code.js:152)
    at <eval> (/src/user_code.js:189)

EXPECTED|[[1,4,7],[2,5,8],[3,6,9]]
TEST_RESULT|1|Runtime Error|0
OUTPUT|TypeEuser_code.js:10929er_code.js:11021er_code.js:11113)
    at <eval> (<input>)
    at run_all_tests (/src/user_code.js:152)
    at <eval> (/src/user_code.js:189)

EXPECTED|[[1,4],[2,5],[3,6]]
TEST_RESULT|2|Runtime Error|0
OUTPUT|TypeEuser_code.js:10929er_code.js:11021er_code.js:11113)
    at <eval> (<input>)
    at run_all_tests (/src/user_code.js:152)
    at <eval> (/src/user_code.js:189)

EXPECTED|
TEST_RESULT|3|Runtime Error|0
OUTPUT|TypeEuser_code.js:10929er_code.js:11021er_code.js:11113)
    at <eval> (<input>)
    at run_all_tests (/src/user_code.js:152)
    at <eval> (/src/user_code.js:189)

EXPECTED|
TEST_RESULT|4|Runtime Error|0
OUTPUT|TypeEuser_code.js:10929er_code.js:11021er_code.js:11113)
    at <eval> (<input>)
    at run_all_tests (/src/user_code.js:152)
    at <eval> (/src/user_code.js:189)

EXPECTED|
TEST_RESULT|5|Runtime Error|0
OUTPUT|TypeEuser_code.js:10929er_code.js:11021er_code.js:11113)
    at <eval> (<input>)
    at run_all_tests (/src/user_code.js:152)
    at <eval> (/src/user_code.js:189)

EXPECTED|
TEST_RESULT|6|Runtime Error|0
OUTPUT|TypeEuser_code.js:10929er_code.js:11021er_code.js:11113)
    at <eval> (<input>)
    at run_all_tests (/src/user_code.js:152)
    at <eval> (/src/user_code.js:189)

EXPECTED|
TEST_RESULT|7|Runtime Error|0
OUTPUT|TypeEuser_code.js:10929er_code.js:11021er_code.js:11113)
    at <eval> (<input>)
    at run_all_tests (/src/user_code.js:152)
    at <eval> (/src/user_code.js:189)

EXPECTED|

```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/easy/transpose_matrix.json"
Loaded 8 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/easy/transpose_matrix.json"
Loaded 8 test cases
JavascriptRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/quickjs", "--command", "qjs", "--", "/src/user_code.js")
JavascriptRunner: waiting for finish...
JavascriptRunner: finished. exitCode: 0
JavascriptRunner: returning result
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 1
PythonRunner: returning result
Python evaluation failed! "Traceback (most recent call last):\n  File \"/src/user_code.py\", line 74, in <module>\n    evaluate()\n  File \"/src/user_code.py\", line 40, in evaluate\n    exec(oracle_code, globals())\n  File \"<string>\", line 3, in <module>\n  File \"<string>\", line 4, in Checker\nTypeError: 'type' object is not subscriptable\n" ""

```
