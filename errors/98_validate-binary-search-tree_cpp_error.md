# Error for Problem 98 (cpp)

## Stdout
```
COMPILATION_ERROR|Harness output did not contain test summary. Output:
DEBUG:{"in":{"root":[2,1,3]},"name":"example_1","out":true}
DEBUG:{"in":{"root":[5,1,4,null,null,3,6]},"name":"example_2","out":false}
DEBUG:{"in":{"root":[1]},"name":"single_node","out":true}
DEBUG:{"in":{"root":[1,null,2]},"name":"two_nodes_valid","out":true}
DEBUG:{"in":{"root":[1,2,null]},"name":"two_nodes_invalid","out":false}
DEBUG:{"in":{"root":[2147483647]},"name":"max_int_edge","out":true}
DEBUG:{"in":{"root":[-2147483648]},"name":"min_int_edge","out":true}
DEBUG:{"in":{"root":[10,5,null,2,7,null,null]},"name":"left_chain_valid","out":true}
DEBUG:{"in":{"root":[10,null,20,null,null,15,25]},"name":"right_chain_valid","out":true}
DEBUG:{"in":{"root":[5,1,6,null,null,3,7]},"name":"invalid_right_of_left","out":false}
DEBUG:{"in":{"root":[5,4,6,null,null,3,7]},"name":"invalid_left_of_right","out":false}
DEBUG:{"in":{"root":[1,1,1]},"name":"all_same_values","out":false}
DEBUG:{"in":{"root":[2,2,2]},"name":"strict_less_than","out":false}
DEBUG:{"in":{"root":[10,5,15,null,8,12,20]},"name":"zig_zag_valid","out":true}
DEBUG:{"in":{"root":[10,5,15,null,12,8,20]},"name":"zig_zag_invalid","out":false}
DEBUG:{"in":{"root":[-2147483648,null,2147483647]},"name":"negative_boundary","out":true}
DEBUG:{"in":{"root":[-2147483648,-2147483648,null]},"name":"negative_boundary_invalid","out":false}
DEBUG:{"in":{"root":[4,2,6,1,3,5,7]},"name":"full_tree_valid","out":true}
DEBUG:{"in":{"root":[4,2,6,1,5,3,7]},"name":"full_tree_invalid","out":false}
DEBUG:{"in":{"root":[10,5,15,2,7]},"name":"left_deep_valid","out":true}
DEBUG:{"in":{"root":[10,5,15,null,null,12,20]},"name":"right_deep_valid","out":true}
DEBUG:{"in":{"root":{"elemType":"int","value":[8]}},"name":"gen_1","out":true,"seed":1}

Error:
error: RuntimeError: unreachable
    at abort (<module>[3399]:0xb7e4a)
    at void nlohmann::json_abi_v3_11_3::detail::from_json<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void>, int, 0>(nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&, int&) (<module>[464]:0x149d4)
    at decltype(from_json(fp, std::forward<int&>(fp0))) nlohmann::json_abi_v3_11_3::detail::from_json_fn::operator()<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void>, int&>(nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&, int&) const (<module>[463]:0x1484b)
    at decltype(nlohmann::json_abi_v3_11_3::from_json(std::forward<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&>(fp), fp0), (void)()) nlohmann::json_abi_v3_11_3::adl_serializer<int, void>::from_json<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&, int>(nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&, int&) (<module>[462]:0x147ea)
    at int nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void>::get_impl<int, 0>(nlohmann::json_abi_v3_11_3::detail::priority_tag<0u>) const (<module>[342]:0x11898)
    at decltype(decltype(std::__declval<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&>(0)) std::__2::declval<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&>()().get_impl<int>(nlohmann::json_abi_v3_11_3::detail::priority_tag<4u>{})) nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void>::get<int, int>() const (<module>[127]:0xa0fd)
    at jsonToOptionalIntArray(nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&) (<module>[39]:0x35ee)
    at __original_main (<module>[16]:0x19c7)
    at _start (<module>[15]:0x157e)
╰─▶ 1: RuntimeError: unreachable
           at abort (<module>[3399]:0xb7e4a)
           at void nlohmann::json_abi_v3_11_3::detail::from_json<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void>, int, 0>(nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&, int&) (<module>[464]:0x149d4)
           at decltype(from_json(fp, std::forward<int&>(fp0))) nlohmann::json_abi_v3_11_3::detail::from_json_fn::operator()<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void>, int&>(nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&, int&) const (<module>[463]:0x1484b)
           at decltype(nlohmann::json_abi_v3_11_3::from_json(std::forward<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&>(fp), fp0), (void)()) nlohmann::json_abi_v3_11_3::adl_serializer<int, void>::from_json<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&, int>(nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&, int&) (<module>[462]:0x147ea)
           at int nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void>::get_impl<int, 0>(nlohmann::json_abi_v3_11_3::detail::priority_tag<0u>) const (<module>[342]:0x11898)
           at decltype(decltype(std::__declval<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&>(0)) std::__2::declval<nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&>()().get_impl<int>(nlohmann::json_abi_v3_11_3::detail::priority_tag<4u>{})) nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void>::get<int, int>() const (<module>[127]:0xa0fd)
           at jsonToOptionalIntArray(nlohmann::json_abi_v3_11_3::basic_json<std::__2::map, std::__2::vector, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>, bool, long long, unsigned long long, double, std::__2::allocator, nlohmann::json_abi_v3_11_3::adl_serializer, std::__2::vector<unsigned char, std::__2::allocator<unsigned char>>, void> const&) (<module>[39]:0x35ee)
           at __original_main (<module>[16]:0x19c7)
           at _start (<module>[15]:0x157e)


```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/validate_binary_search_tree.json"
Loaded 34 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/validate_binary_search_tree.json"
Loaded 34 test cases
WasmRunner: starting compileProc... args: QList("-I/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/resources/include", "-fno-exceptions", "-fno-rtti", "-DJSON_HAS_FILESYSTEM=0", "-DJSON_HAS_EXPERIMENTAL_FILESYSTEM=0", "-include-pch", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/resources/include/stdcpp.h.pch", "user_code.cpp", "-o", "user_bin.wasm")
WasmRunner: waiting for compile finish...
WasmRunner: compile finished. exitCode: 0
WasmRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "user_bin.wasm")
WasmRunner: waiting for finish...
WasmRunner: finished. exitCode: 1
WasmRunner: returning result

```
