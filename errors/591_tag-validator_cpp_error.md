# Error for Problem 591 (cpp)

## Stdout
```
COMPILATION_ERROR|user_code.cpp:4:9: error: unknown type name 'regex'
        regex tag_pat(R"(^<(/?)([A-Z]{1,9})>$)");
        ^
user_code.cpp:315:13: error: unknown type name 'smatch'
            smatch m;
            ^
2 errors generated.


```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/hard/tag_validator.json"
Loaded 36 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/hard/tag_validator.json"
Loaded 36 test cases
WasmRunner: starting compileProc... args: QList("-I/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/resources/include", "-fno-exceptions", "-fno-rtti", "-DJSON_HAS_FILESYSTEM=0", "-DJSON_HAS_EXPERIMENTAL_FILESYSTEM=0", "-include-pch", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/resources/include/stdcpp.h.pch", "user_code.cpp", "-o", "user_bin.wasm")
WasmRunner: waiting for compile finish...
WasmRunner: compile finished. exitCode: 1

```
