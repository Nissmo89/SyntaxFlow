# Error for Problem 1002 (cpp)

## Stdout
```
TEST_RESULT|0|Accepted|139
TEST_RESULT|1|Accepted|139
TEST_RESULT|2|Accepted|139
TEST_RESULT|3|Wrong Answer|139
OUTPUT|["d","b"]
EXPECTED|["b","d"]
TEST_RESULT|4|Accepted|139
TEST_RESULT|5|Wrong Answer|139
OUTPUT|["b","a","c","c"]
EXPECTED|["c","c","a","b"]
TEST_RESULT|6|Accepted|139
TEST_RESULT|7|Accepted|139
TEST_RESULT|8|Accepted|139
TEST_RESULT|9|Accepted|139
TEST_RESULT|10|Accepted|139
TEST_RESULT|11|Wrong Answer|139
OUTPUT|["a","c"]
EXPECTED|["c","a"]
TEST_RESULT|12|Wrong Answer|139
OUTPUT|["c","c","d"]
EXPECTED|["d","c","c"]
TEST_RESULT|13|Accepted|139
TEST_RESULT|14|Accepted|139
TEST_RESULT|15|Accepted|139
TEST_RESULT|16|Accepted|139
TEST_RESULT|17|Accepted|139
TEST_RESULT|18|Accepted|139
TEST_RESULT|19|Accepted|139
TEST_RESULT|20|Accepted|139
TEST_RESULT|21|Accepted|139
TEST_RESULT|22|Wrong Answer|139
OUTPUT|["e","e","c"]
EXPECTED|["c","e","e"]
TEST_RESULT|23|Accepted|139
TEST_RESULT|24|Wrong Answer|139
OUTPUT|["a","b","b","e","d","d"]
EXPECTED|["d","d","e","b","b","a"]
TEST_RESULT|25|Wrong Answer|139
OUTPUT|["e","c"]
EXPECTED|["c","e"]
TEST_RESULT|26|Accepted|139
TEST_RESULT|27|Accepted|139
TEST_RESULT|28|Accepted|139
TEST_RESULT|29|Accepted|139
TEST_RESULT|30|Accepted|139
TEST_RESULT|31|Accepted|139
TEST_RESULT|32|Accepted|139
TEST_RESULT|33|Accepted|139
TEST_RESULT|34|Accepted|139

```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/easy/find_common_characters.json"
Loaded 35 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/easy/find_common_characters.json"
Loaded 35 test cases
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
