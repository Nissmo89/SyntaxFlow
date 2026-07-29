# Error for Problem 831 (cpp)

## Stdout
```
COMPILATION_ERROR|Harness output did not contain test summary. Output:
DEBUG:{"in":{"s":"LeetCode@LeetCode.com"},"name":"ex1","out":"l*****e@leetcode.com"}
DEBUG:{"in":{"s":"AB@qq.com"},"name":"ex2","out":"a*****b@qq.com"}
DEBUG:{"in":{"s":"1(234)567-890"},"name":"ex3","out":"***-***-7890"}
DEBUG:{"in":{"s":"Aa@Bb.cc"},"name":"email-short","out":"a*****a@bb.cc"}
DEBUG:{"in":{"s":"John.Smith@Example.COM"},"name":"email-mixed","out":"j*****h@example.com"}
DEBUG:{"in":{"s":"aB@cD.Ef"},"name":"email-lower","out":"a*****b@cd.ef"}
DEBUG:{"in":{"s":"ZYXWVUT@QWERTY.AS"},"name":"email-upper","out":"z*****t@qwerty.as"}
DEBUG:{"in":{"s":1234567890},"name":"phone-10-digits","out":"***-***-7890"}

Error:
error: RuntimeError: unreachable
    at abort (<module>[3133]:0xb054f)
    at std::__2::__throw_out_of_range[abi:v160000](char const*) (<module>[1695]:0x4ece3)
    at std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>::__throw_out_of_range[abi:v160000]() const (<module>[1690]:0x4e6aa)
    at std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>::basic_string(std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>> const&, unsigned long, unsigned long, std::__2::allocator<char> const&) (<module>[1712]:0x503b6)
    at std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>::substr[abi:v160000](unsigned long, unsigned long) const (<module>[109]:0x9d16)
    at Solution::maskPII(std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>) (<module>[44]:0x4171)
    at __original_main (<module>[16]:0x1a06)
    at _start (<module>[15]:0x1475)
╰─▶ 1: RuntimeError: unreachable
           at abort (<module>[3133]:0xb054f)
           at std::__2::__throw_out_of_range[abi:v160000](char const*) (<module>[1695]:0x4ece3)
           at std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>::__throw_out_of_range[abi:v160000]() const (<module>[1690]:0x4e6aa)
           at std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>::basic_string(std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>> const&, unsigned long, unsigned long, std::__2::allocator<char> const&) (<module>[1712]:0x503b6)
           at std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>::substr[abi:v160000](unsigned long, unsigned long) const (<module>[109]:0x9d16)
           at Solution::maskPII(std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>) (<module>[44]:0x4171)
           at __original_main (<module>[16]:0x1a06)
           at _start (<module>[15]:0x1475)


```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/masking_personal_information.json"
Loaded 35 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/masking_personal_information.json"
Loaded 35 test cases
WasmRunner: starting compileProc... args: QList("-I/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/resources/include", "-fno-exceptions", "-fno-rtti", "-DJSON_HAS_FILESYSTEM=0", "-DJSON_HAS_EXPERIMENTAL_FILESYSTEM=0", "-include-pch", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/resources/include/stdcpp.h.pch", "user_code.cpp", "-o", "user_bin.wasm")
WasmRunner: waiting for compile finish...
WasmRunner: compile finished. exitCode: 0
WasmRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "user_bin.wasm")
WasmRunner: waiting for finish...
WasmRunner: finished. exitCode: 1
WasmRunner: returning result

```
