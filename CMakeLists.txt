cmake_minimum_required(VERSION 3.19)
project(project_syntaxFlow LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FIX 1: Added Gui to the components list
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Widgets Gui)
qt_standard_project_setup()

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

# FIX: Tree-sitter-cpp's scanner often requires C11 or to be treated as C++
# We will set it to C11 and ensure it has access to the core headers
target_include_directories(tree-sitter-cpp PUBLIC
    libs/tree-sitter-cpp/src
)
target_link_libraries(tree-sitter-cpp PRIVATE tree-sitter)

# This forces the compiler to use C11, which understands static_assert
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


# FIX 2: Linked Qt::Gui here so <QtGui> or <QSyntaxHighlighter> can be found
target_link_libraries(tree-sitter-highlighter PUBLIC
    # ${CMAKE_CURRENT_SOURCE_DIR}/src
    tree-sitter
    tree-sitter-cpp
    Qt::Core
    Qt::Gui
    Qt::Widgets   # ← THIS WAS MISSING
)

# --- Main Executable ---
qt_add_executable(project_syntaxFlow
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    hoversidebar.cpp hoversidebar.h
    problemwidgets.cpp problemwidgets.h
    problem_panel.h problem_panel.cpp
    jsonutils.h jsonutils.cpp
    code_editor.h code_editor.cpp
    testcase_panel.h testcase_panel.cpp
    language_config.h language_config.cpp
    language_registry.h language_registry.cpp
    code_runner.h code_runner.cpp
    backend.h backend.cpp
    output_normalizer.h
)

# FIX 3: Ensure the main app also has access to the Gui symbols
target_link_libraries(project_syntaxFlow PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Gui
    tree-sitter-highlighter
)
