cmake_minimum_required(VERSION 3.19)
project(SyntaxFlowHeadless LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_executable(headless_test_runner
    headless_test_runner.cpp
    language_config.cpp
    language_registry.cpp
    code_runner.cpp
    driver_generator.cpp
    progressmanager.cpp
    python_runner.cpp
    javascript_runner.cpp
    wasm_runner.cpp
)

target_link_libraries(headless_test_runner PRIVATE
    Qt6::Core
    Qt6::Network
)
