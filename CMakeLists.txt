cmake_minimum_required(VERSION 3.19)
project(SyntaxFlow LANGUAGES C CXX)

# ---- C++ & Qt Setup ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---- Find Qt ----
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

# ============================================================
# QText Browser Custom Renderer Module
# ============================================================
add_subdirectory(libs/qtext_browser)

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
# Python 3 (Required)
# ============================================================
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Python3 found: ${Python3_VERSION} — ${Python3_EXECUTABLE}")

# ============================================================
# Main Executable
# ============================================================
add_executable(SyntaxFlow
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    hoversidebar.cpp hoversidebar.h
    problemwidgets.cpp problemwidgets.h
    problem_panel.cpp problem_panel.h
    markdownrenderer.cpp markdownrenderer.h
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
    python_runner.h python_runner.cpp
)

target_include_directories(SyntaxFlow PRIVATE ${Python3_INCLUDE_DIRS})

# ---- Link Libraries ----
target_link_libraries(SyntaxFlow PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    tree-sitter-highlighter
    qtext_browser
    ${Python3_LIBRARIES}
)

if(WIN32)
    # MinGW needs these for Python on Windows
    target_link_libraries(SyntaxFlow PRIVATE ws2_32 winmm)
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
elseif(UNIX AND NOT APPLE AND EXISTS ${QSCI_LOCAL_LINUX})
    message(STATUS "Using local QScintilla (Linux): ${QSCI_LOCAL_LINUX}")
    target_link_libraries(SyntaxFlow PRIVATE ${QSCI_LOCAL_LINUX})
else()
    # BUILD-03: Try pkg-config first (most reliable across distros)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(QSCI QUIET qscintilla2-qt6)
    endif()

    if(QSCI_FOUND)
        message(STATUS "QScintilla found via pkg-config: ${QSCI_LIBRARIES}")
        target_link_libraries(SyntaxFlow PRIVATE ${QSCI_LIBRARIES})
        target_include_directories(SyntaxFlow PRIVATE ${QSCI_INCLUDE_DIRS})
    else()
        find_library(QSCI_SYSTEM_LIB
            NAMES qscintilla2_qt6 qscintilla2-qt6 qscintilla2 libqscintilla2
        )
        if(QSCI_SYSTEM_LIB)
            message(STATUS "Using system QScintilla: ${QSCI_SYSTEM_LIB}")
            target_link_libraries(SyntaxFlow PRIVATE ${QSCI_SYSTEM_LIB})
        else()
            message(FATAL_ERROR
                "QScintilla not found locally or system-wide. Install it:\n"
                "  Ubuntu/Debian: sudo apt install libqscintilla2-qt6-dev\n"
                "  Fedora:        sudo dnf install qscintilla-qt6-devel\n"
                "  Arch:          sudo pacman -S qscintilla-qt6\n"
                "  Windows:       Run build-windows.bat (builds from source in libs/)")
        endif()
    endif()
endif()

# ============================================================
# Copy runtime assets after build
# ============================================================
add_custom_command(TARGET SyntaxFlow POST_BUILD
    COMMENT "Copying runtime assets..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/problems"
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/problems.json"
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems.json"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/qtext_browser/style.css"
        "$<TARGET_FILE_DIR:SyntaxFlow>/style.css"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/qtext_browser/fonts"
        "$<TARGET_FILE_DIR:SyntaxFlow>/fonts"
)


# ---- Windows: hide console ----
if(WIN32)
    set_target_properties(SyntaxFlow PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
