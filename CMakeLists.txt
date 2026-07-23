cmake_minimum_required(VERSION 3.19)
project(SyntaxFlow LANGUAGES C CXX)

# ---- C++ & Qt Setup ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---- Find Qt ----
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network WebEngineWidgets WebChannel)

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
    web_workspace.cpp web_workspace.h
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
    Qt6::WebEngineWidgets
    Qt6::WebChannel
    tree-sitter-highlighter
    qtext_browser
    ${Python3_LIBRARIES}
)

if(WIN32)
    # MinGW needs these for Python on Windows
    target_link_libraries(SyntaxFlow PRIVATE ws2_32 winmm)
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
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/web_editor"
        "$<TARGET_FILE_DIR:SyntaxFlow>/web_editor"
)


# ---- Windows: hide console ----
if(WIN32)
    set_target_properties(SyntaxFlow PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
