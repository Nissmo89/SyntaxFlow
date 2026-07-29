# Error for Problem 282 (cpp)

## Stdout
```
TEST_RESULT|0|Wrong Answer|277
OUTPUT|["1+2+3","1*2*3"]
EXPECTED|["1*2*3","1+2+3"]
TEST_RESULT|1|Wrong Answer|277
OUTPUT|["2+3*2","2*3+2"]
EXPECTED|["2*3+2","2+3*2"]
TEST_RESULT|2|Accepted|277
TEST_RESULT|3|Accepted|277
TEST_RESULT|4|Accepted|277
TEST_RESULT|5|Accepted|277
TEST_RESULT|6|Accepted|277
TEST_RESULT|7|Accepted|277
TEST_RESULT|8|Accepted|277
TEST_RESULT|9|Accepted|277
TEST_RESULT|10|Accepted|277
TEST_RESULT|11|Accepted|277
TEST_RESULT|12|Accepted|277
TEST_RESULT|13|Accepted|277
TEST_RESULT|14|Accepted|277
TEST_RESULT|15|Accepted|277
TEST_RESULT|16|Accepted|277
TEST_RESULT|17|Accepted|277
TEST_RESULT|18|Accepted|277
TEST_RESULT|19|Accepted|277
TEST_RESULT|20|Accepted|277
TEST_RESULT|21|Accepted|277
TEST_RESULT|22|Accepted|277
TEST_RESULT|23|Accepted|277
TEST_RESULT|24|Accepted|277
TEST_RESULT|25|Accepted|277
TEST_RESULT|26|Accepted|277
TEST_RESULT|27|Accepted|277
TEST_RESULT|28|Accepted|277
TEST_RESULT|29|Accepted|277
TEST_RESULT|30|Accepted|277
TEST_RESULT|31|Accepted|277
TEST_RESULT|32|Accepted|277
TEST_RESULT|33|Accepted|277
TEST_RESULT|34|Accepted|277
TEST_RESULT|35|Accepted|277
TEST_RESULT|36|Accepted|277
TEST_RESULT|37|Accepted|277
TEST_RESULT|38|Accepted|277

```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/hard/expression_add_operators.json"
Loaded 39 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/hard/expression_add_operators.json"
Loaded 39 test cases
WasmRunner: starting compileProc... args: QList("-I/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/resources/include", "-fno-exceptions", "-fno-rtti", "-DJSON_HAS_FILESYSTEM=0", "-DJSON_HAS_EXPERIMENTAL_FILESYSTEM=0", "-include-pch", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/resources/include/stdcpp.h.pch", "user_code.cpp", "-o", "user_bin.wasm")
WasmRunner: waiting for compile finish...
WasmRunner: compile finished. exitCode: 0
WasmRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "user_bin.wasm")
WasmRunner: waiting for finish...
WasmRunner: finished. exitCode: 0
WasmRunner: returning result
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 0
PythonRunner: returning result

```
