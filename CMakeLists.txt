cmake_minimum_required(VERSION 3.19)
project(SyntaxFlow LANGUAGES C CXX)

# 1. Qt 6.2 & C++ Standard Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 2. Find Qt 6.2
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets)

# --- Tree-sitter Core ---
add_library(tree-sitter STATIC
    libs/tree-sitter/lib/src/lib.c
)
target_include_directories(tree-sitter PUBLIC
    libs/tree-sitter/lib/include
    libs/tree-sitter/lib/src
)
set_target_properties(tree-sitter PROPERTIES C_STANDARD 99)

# --- Tree-sitter C++ Grammar ---
add_library(tree-sitter-cpp STATIC
    libs/tree-sitter-cpp/src/parser.c
    libs/tree-sitter-cpp/src/scanner.c
)
target_include_directories(tree-sitter-cpp PUBLIC
    libs/tree-sitter-cpp/src
)
target_link_libraries(tree-sitter-cpp PRIVATE tree-sitter)

# Force C11 for parser
set_target_properties(tree-sitter-cpp PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# --- Highlighter Logic ---
add_library(tree-sitter-highlighter STATIC
    src/TreeSitterHighlighter.cpp
    src/TreeSitterHighlighter.h
)
target_include_directories(tree-sitter-highlighter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(tree-sitter-highlighter PUBLIC
    tree-sitter
    tree-sitter-cpp
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# --- Main Executable ---
add_executable(SyntaxFlow
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    hoversidebar.cpp hoversidebar.h
    problemwidgets.cpp problemwidgets.h
    problem_panel.cpp problem_panel.h
    jsonutils.cpp jsonutils.h
    code_editor.cpp code_editor.h
    testcase_panel.cpp testcase_panel.h
    language_config.cpp language_config.h
    language_registry.cpp language_registry.h
    code_runner.cpp code_runner.h
    backend.cpp backend.h
    output_normalizer.h
)

target_link_libraries(SyntaxFlow PRIVATE
    tree-sitter-highlighter
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Copy assets after build
add_custom_command(TARGET SyntaxFlow POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/problems"
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/problems.json"
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems.json"
    COMMENT "Copying problems directory and JSON to build folder..."
)

# Windows: Hide console
set_target_properties(SyntaxFlow PROPERTIES
    WIN32_EXECUTABLE TRUE
)
