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
# QuickJS (Static Embedded)
# ============================================================
set(QUICKJS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/quickjs)

add_library(quickjs STATIC
    ${QUICKJS_ROOT}/quickjs.c
    ${QUICKJS_ROOT}/libregexp.c
    ${QUICKJS_ROOT}/libunicode.c
    ${QUICKJS_ROOT}/cutils.c
    ${QUICKJS_ROOT}/dtoa.c
)

target_include_directories(quickjs PUBLIC ${QUICKJS_ROOT})

# Rename conflicting symbols to avoid clash with libtcc
target_compile_definitions(quickjs PRIVATE
    CONFIG_VERSION="2024-01-13"
    "pstrcpy=qjs_pstrcpy"
    "pstrcat=qjs_pstrcat"
    "unicode_to_utf8=qjs_unicode_to_utf8"
)

set_target_properties(quickjs PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)

if(MSVC)
    target_compile_options(quickjs PRIVATE
        /wd4244   # conversion, possible loss of data
        /wd4267   # size_t to int conversion
        /wd4996   # deprecated POSIX names
        /wd4018   # signed/unsigned mismatch
    )
elseif(MINGW)
    target_compile_options(quickjs PRIVATE
        -Wno-implicit-function-declaration
        -Wno-sign-compare
        -Wno-unused-variable
        -Wno-unused-parameter
    )
else()
    target_compile_options(quickjs PRIVATE
        -Wno-implicit-function-declaration
        -Wno-sign-compare
    )
endif()

# ============================================================
# Python 3 (Optional — disable python_runner if not found)
# ============================================================
# On Windows with MinGW, set Python3_ROOT_DIR via cmake preset or
# -DPYTHON3_ROOT_DIR=C:/Python312 on the command line.
if(WIN32)
    find_package(Python3 COMPONENTS Interpreter Development QUIET)
else()
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
endif()

if(Python3_FOUND)
    message(STATUS "Python3 found: ${Python3_VERSION} — ${Python3_EXECUTABLE}")
    set(SF_ENABLE_PYTHON ON)
else()
    message(WARNING "Python3 Development libs not found. python_runner will be disabled.")
    set(SF_ENABLE_PYTHON OFF)
endif()

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
    driver_generator.cpp driver_generator.h
    backend.cpp backend.h
    output_normalizer.h
    progressmanager.h progressmanager.cpp
    embedded_runner.h
    tcc_compiler.h tcc_compiler.cpp
    quickjs_runner.h quickjs_runner.cpp
    lab_manager.h lab_manager.cpp
    lab_browser.h lab_browser.cpp
    output_panel.h output_panel.cpp
)

# Conditionally include python runner
if(SF_ENABLE_PYTHON)
    target_sources(SyntaxFlow PRIVATE python_runner.h python_runner.cpp)
    target_compile_definitions(SyntaxFlow PRIVATE SF_PYTHON_ENABLED)
    target_include_directories(SyntaxFlow PRIVATE ${Python3_INCLUDE_DIRS})
endif()

# ---- Link Qt ----
target_link_libraries(SyntaxFlow PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    tree-sitter-highlighter
    quickjs
)

# ---- Link Python if found ----
if(SF_ENABLE_PYTHON)
    target_link_libraries(SyntaxFlow PRIVATE ${Python3_LIBRARIES})
    if(WIN32)
        # MinGW needs these for Python on Windows
        target_link_libraries(SyntaxFlow PRIVATE ws2_32 winmm)
    endif()
endif()

# ============================================================
# QScintilla (Manual first, fallback system)
# ============================================================
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
    message(STATUS "Using local QScintilla (Windows): ${QSCI_LOCAL_WIN}")
    target_link_libraries(SyntaxFlow PRIVATE ${QSCI_LOCAL_WIN})
elseif(UNIX AND EXISTS ${QSCI_LOCAL_LINUX})
    message(STATUS "Using local QScintilla (Linux): ${QSCI_LOCAL_LINUX}")
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

# ============================================================
# TinyCC (Static Embedded)
# ============================================================
set(TINYCC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinycc)

if(WIN32)
    set(TINYCC_LIB ${TINYCC_ROOT}/windows/libtcc.a)
elseif(UNIX)
    set(TINYCC_LIB ${TINYCC_ROOT}/linux/libtcc.a)
endif()

if(EXISTS ${TINYCC_LIB})
    message(STATUS "Using static TinyCC: ${TINYCC_LIB}")

    add_library(tcc STATIC IMPORTED)
    set_target_properties(tcc PROPERTIES
        IMPORTED_LOCATION ${TINYCC_LIB}
        INTERFACE_INCLUDE_DIRECTORIES ${TINYCC_ROOT}
    )
    target_link_libraries(SyntaxFlow PRIVATE tcc)

    # On MinGW, TinyCC's internal relocation may need dl-like linkage.
    # Also ensure we don't strip exports (needed for tcc_get_symbol).
    if(WIN32 AND MINGW)
        target_link_libraries(SyntaxFlow PRIVATE -Wl,--export-all-symbols)
    endif()

else()
    message(FATAL_ERROR
        "TinyCC static library not found at: ${TINYCC_LIB}\n"
        "On Windows: run build-windows.bat to build libtcc first.\n"
        "On Linux:   run libs/tinycc/configure && make first.")
endif()

# ============================================================
# Copy runtime assets after build
# ============================================================
if(WIN32)
    add_custom_command(TARGET SyntaxFlow POST_BUILD
        COMMENT "Copying Windows TCC runtime assets..."
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SyntaxFlow>/tcc"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${TINYCC_ROOT}/windows/libtcc1.a"
            "$<TARGET_FILE_DIR:SyntaxFlow>/tcc"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${TINYCC_ROOT}/include"
            "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/include"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/problems"
            "$<TARGET_FILE_DIR:SyntaxFlow>/problems"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/problems.json"
            "$<TARGET_FILE_DIR:SyntaxFlow>/problems.json"
    )
else()
    add_custom_command(TARGET SyntaxFlow POST_BUILD
        COMMENT "Copying Linux TCC runtime assets..."
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SyntaxFlow>/tcc"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${TINYCC_ROOT}/linux/libtcc1.a"
            "${TINYCC_ROOT}/linux/runmain.o"
            "${TINYCC_ROOT}/linux/bt-exe.o"
            "${TINYCC_ROOT}/linux/bt-log.o"
            "${TINYCC_ROOT}/linux/bcheck.o"
            "$<TARGET_FILE_DIR:SyntaxFlow>/tcc"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${TINYCC_ROOT}/include"
            "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/include"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/problems"
            "$<TARGET_FILE_DIR:SyntaxFlow>/problems"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/problems.json"
            "$<TARGET_FILE_DIR:SyntaxFlow>/problems.json"
    )
endif()

# ---- Windows: hide console ----
if(WIN32)
    set_target_properties(SyntaxFlow PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
