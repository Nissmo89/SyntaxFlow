cmake_minimum_required(VERSION 3.19)
project(SyntaxFlow LANGUAGES C CXX)

# ---- C++ & Qt Setup ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---- Find Qt ----
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# ============================================================
# Tree-sitter Core
# ============================================================
add_library(tree-sitter STATIC
    libs/tree-sitter/lib/src/lib.c
)
target_include_directories(tree-sitter PUBLIC
    libs/tree-sitter/lib/include
    libs/tree-sitter/lib/src
)
set_target_properties(tree-sitter PROPERTIES C_STANDARD 99)

# ============================================================
# Tree-sitter C++ Grammar
# ============================================================
add_library(tree-sitter-cpp STATIC
    libs/tree-sitter-cpp/src/parser.c
    libs/tree-sitter-cpp/src/scanner.c
)
target_include_directories(tree-sitter-cpp PUBLIC
    libs/tree-sitter-cpp/src
)
target_link_libraries(tree-sitter-cpp PRIVATE tree-sitter)
set_target_properties(tree-sitter-cpp PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# ============================================================
# Highlighter
# ============================================================
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

# ============================================================
# Main Executable
# ============================================================
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
    progressmanager.h progressmanager.cpp
)

# ---- Link Qt BEFORE QScintilla ----
target_link_libraries(SyntaxFlow PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    tree-sitter-highlighter
)

# ============================================================
# QScintilla (Manual first, fallback system)
# ============================================================
# Include headers always from local source (if present)
target_include_directories(SyntaxFlow PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qscintilla/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qscintilla/scintilla/include
)

set(QSCI_LOCAL_WIN
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qscintilla/src/release/libqscintilla2_qt6.a
)
set(QSCI_LOCAL_LINUX
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qscintilla/src/libqscintilla2.so
)

if(WIN32 AND EXISTS ${QSCI_LOCAL_WIN})
    message(STATUS "Using local QScintilla (Windows)")
    target_link_libraries(SyntaxFlow PRIVATE ${QSCI_LOCAL_WIN})
elseif(UNIX AND EXISTS ${QSCI_LOCAL_LINUX})
    message(STATUS "Using local QScintilla (Linux)")
    target_link_libraries(SyntaxFlow PRIVATE ${QSCI_LOCAL_LINUX})
else()
    message(STATUS "Local QScintilla not found. Trying system/Qt installation...")
    find_library(QSCI_SYSTEM_LIB
        NAMES qscintilla2 qscintilla2_qt6
    )
    if(QSCI_SYSTEM_LIB)
        message(STATUS "Using system QScintilla: ${QSCI_SYSTEM_LIB}")
        target_link_libraries(SyntaxFlow PRIVATE ${QSCI_SYSTEM_LIB})
    else()
        message(FATAL_ERROR "QScintilla not found locally or system-wide.")
    endif()
endif()

# ---- Copy assets after build ----
add_custom_command(TARGET SyntaxFlow POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/problems"
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/problems.json"
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems.json"
)

# ---- Windows: hide console ----
set_target_properties(SyntaxFlow PROPERTIES
    WIN32_EXECUTABLE TRUE
)
