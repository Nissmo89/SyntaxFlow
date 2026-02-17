cmake_minimum_required(VERSION 3.19)
project(SyntaxFlow LANGUAGES C CXX)

# ============================================================
# C++ & Qt Setup
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# ============================================================
# Platform & Architecture Detection
# ============================================================
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(TCC_ARCH "x86_64")
    set(TCC_ARCH_BITS 64)
    set(TCC_MULTIARCH "x86_64-linux-gnu")
else()
    set(TCC_ARCH "i386")
    set(TCC_ARCH_BITS 32)
    set(TCC_MULTIARCH "i386-linux-gnu")
endif()

if(WIN32)
    set(TCC_PLATFORM "win32")
else()
    set(TCC_PLATFORM "linux")
endif()

message(STATUS "")
message(STATUS "╔══════════════════════════════════════════╗")
message(STATUS "║       SyntaxFlow Build Configuration     ║")
message(STATUS "╠══════════════════════════════════════════╣")
message(STATUS "║  Platform:     ${CMAKE_SYSTEM_NAME}")
message(STATUS "║  Architecture: ${TCC_ARCH} (${TCC_ARCH_BITS}-bit)")
message(STATUS "║  Compiler:     ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "║  Qt Version:   ${Qt6_VERSION}")
if(NOT WIN32)
    message(STATUS "║  Multiarch:    ${TCC_MULTIARCH}")
endif()
message(STATUS "╚══════════════════════════════════════════╝")
message(STATUS "")

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
# Tree-sitter Highlighter
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
# libtcc (TinyCC) - Cross-Platform Configuration
# ============================================================
set(TCC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinycc)
set(TCC_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/tcc)
set(TCC_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/tcc_output)

file(MAKE_DIRECTORY ${TCC_BUILD_DIR})
file(MAKE_DIRECTORY ${TCC_OUTPUT_DIR})
file(MAKE_DIRECTORY ${TCC_OUTPUT_DIR}/include)
file(MAKE_DIRECTORY ${TCC_OUTPUT_DIR}/lib)

# ============================================================
# Generate TCC config.h
# ============================================================
set(TCC_VERSION "0.9.27")

if(WIN32)
    set(CONFIG_TCC_CRTPREFIX "tcc/lib")
    set(CONFIG_TCC_LIBPATHS "tcc/lib")
    set(CONFIG_TCC_SYSINCLUDEPATHS "tcc/include")
    set(CONFIG_TCC_ELFINTERP "")
else()
    # Linux paths with multiarch support
    set(CONFIG_TCC_CRTPREFIX "/usr/lib/${TCC_MULTIARCH}:/usr/lib:/lib/${TCC_MULTIARCH}")
    set(CONFIG_TCC_LIBPATHS "{B}:{B}/lib:tcc/lib:/usr/lib/${TCC_MULTIARCH}:/usr/lib:/lib/${TCC_MULTIARCH}")
    set(CONFIG_TCC_SYSINCLUDEPATHS "{B}/include:tcc/include:/usr/include:/usr/local/include")
    if(TCC_ARCH STREQUAL "x86_64")
        set(CONFIG_TCC_ELFINTERP "/lib64/ld-linux-x86-64.so.2")
    else()
        set(CONFIG_TCC_ELFINTERP "/lib/ld-linux.so.2")
    endif()
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tcc_config.h.in
    ${TCC_BUILD_DIR}/config.h
    @ONLY
)

# ============================================================
# TCC Core Sources (all platforms)
# ============================================================
set(TCC_CORE_SOURCES
    ${TCC_DIR}/libtcc.c
    ${TCC_DIR}/tccpp.c
    ${TCC_DIR}/tccgen.c
    ${TCC_DIR}/tccdbg.c
    ${TCC_DIR}/tccasm.c
    ${TCC_DIR}/tccrun.c
)

# ============================================================
# Architecture-Specific Sources
# ============================================================
if(TCC_ARCH STREQUAL "x86_64")
    list(APPEND TCC_CORE_SOURCES
        ${TCC_DIR}/x86_64-gen.c
        ${TCC_DIR}/x86_64-link.c
        ${TCC_DIR}/i386-asm.c   # <--- ADD THIS LINE
    )
else()
    list(APPEND TCC_CORE_SOURCES
        ${TCC_DIR}/i386-gen.c
        ${TCC_DIR}/i386-link.c
        ${TCC_DIR}/i386-asm.c
    )
endif()

# ============================================================
# Platform-Specific Sources
#
# CRITICAL:
#   - Linux  → tccelf.c ONLY (ELF format)
#   - Windows → tccpe.c + tcccoff.c (PE + COFF formats)
#
# DO NOT mix these! PE/COFF are Windows-only formats.
# ============================================================
if(WIN32)
    message(STATUS "TCC: Adding Windows sources (PE + ELF Core)")
    list(APPEND TCC_CORE_SOURCES
        ${TCC_DIR}/tccelf.c     # REQUIRED: TCC uses ELF internally for symbol management
        ${TCC_DIR}/tccpe.c      # REQUIRED: For Windows PE output
        ${TCC_DIR}/tcctools.c   # REQUIRED: Windows-specific tools
    )
    # We leave tcccoff.c out because it's causing the header conflict
else()
    message(STATUS "TCC: Adding Linux sources (ELF)")
    list(APPEND TCC_CORE_SOURCES
        ${TCC_DIR}/tccelf.c
    )
endif()

# ============================================================
# Build libtcc Static Library
# ============================================================
add_library(libtcc STATIC ${TCC_CORE_SOURCES})

# IMPORTANT: Generated config.h directory FIRST so it takes precedence
target_include_directories(libtcc PUBLIC
    ${TCC_BUILD_DIR}   # Generated config.h - MUST BE FIRST
    ${TCC_DIR}         # TCC source headers
)

set_target_properties(libtcc PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

# ============================================================
# TCC Compile Definitions
# ============================================================
set(TCC_COMPILE_DEFS
    ONE_SOURCE=0
    TCC_LIBTCC1="libtcc1.a"
    CONFIG_TCC_STATIC=1
)

# Architecture definitions
if(TCC_ARCH STREQUAL "x86_64")
    list(APPEND TCC_COMPILE_DEFS TCC_TARGET_X86_64=1)
else()
    list(APPEND TCC_COMPILE_DEFS TCC_TARGET_I386=1)
endif()

# Platform definitions
if(WIN32)
    list(APPEND TCC_COMPILE_DEFS
        TCC_TARGET_PE=1
        _WIN32=1
        CONFIG_WIN32=1
    )
    if(MINGW)
        list(APPEND TCC_COMPILE_DEFS
            __MINGW32__=1
            CONFIG_TCC_CROSSPREFIX=""
        )
    endif()
else()
    list(APPEND TCC_COMPILE_DEFS
        TCC_TARGET_ELF=1
        CONFIG_LDDIR="lib"
        CONFIG_MULTIARCHDIR="${TCC_MULTIARCH}"
    )
endif()

target_compile_definitions(libtcc PRIVATE ${TCC_COMPILE_DEFS})

# Compiler warnings (suppress noisy TCC warnings)
if(MSVC)
    target_compile_options(libtcc PRIVATE /W3)
else()
    target_compile_options(libtcc PRIVATE
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -Wno-sign-compare
        -Wno-pointer-sign
        -Wno-implicit-fallthrough
        -Wno-stringop-truncation
        -Wno-maybe-uninitialized
    )
endif()

# ============================================================
# libtcc1.a Runtime Library
# ============================================================
option(BUILD_TCC_RUNTIME "Build TCC runtime library (libtcc1.a)" ON)

if(BUILD_TCC_RUNTIME)
    set(LIBTCC1_SOURCES "")

    # Core runtime source
    if(EXISTS ${TCC_DIR}/lib/libtcc1.c)
        list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/lib/libtcc1.c)
    endif()

    if(WIN32)
        # Windows runtime sources
        if(EXISTS ${TCC_DIR}/lib/alloca.S)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/lib/alloca.S)
        endif()
        if(TCC_ARCH STREQUAL "x86_64" AND EXISTS ${TCC_DIR}/lib/va_list.c)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/lib/va_list.c)
        endif()
        # Windows CRT support
        if(EXISTS ${TCC_DIR}/win32/lib/chkstk.S)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/win32/lib/chkstk.S)
        endif()
        if(EXISTS ${TCC_DIR}/win32/lib/crt1.c)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/win32/lib/crt1.c)
        endif()
    else()
        # Linux runtime sources
        if(EXISTS ${TCC_DIR}/lib/alloca.S)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/lib/alloca.S)
        endif()
        if(EXISTS ${TCC_DIR}/lib/alloca-bt.S)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/lib/alloca-bt.S)
        endif()
        if(EXISTS ${TCC_DIR}/lib/bcheck.c)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/lib/bcheck.c)
        endif()
        if(TCC_ARCH STREQUAL "x86_64" AND EXISTS ${TCC_DIR}/lib/va_list.c)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/lib/va_list.c)
        endif()
        if(EXISTS ${TCC_DIR}/lib/dsohandle.c)
            list(APPEND LIBTCC1_SOURCES ${TCC_DIR}/lib/dsohandle.c)
        endif()
    endif()

    if(LIBTCC1_SOURCES)
        message(STATUS "TCC: Building libtcc1.a runtime")

        add_library(libtcc1_objs OBJECT ${LIBTCC1_SOURCES})

        # config.h first here too
        target_include_directories(libtcc1_objs PRIVATE
            ${TCC_BUILD_DIR}
            ${TCC_DIR}
            ${TCC_DIR}/include
        )

        set_target_properties(libtcc1_objs PROPERTIES
            C_STANDARD 99
            POSITION_INDEPENDENT_CODE ON
        )

        if(NOT MSVC)
            target_compile_options(libtcc1_objs PRIVATE
                -w
                -fno-builtin
                -fno-stack-protector
            )

            # Handle assembly files
            set_source_files_properties(
                ${TCC_DIR}/lib/alloca.S
                ${TCC_DIR}/lib/alloca-bt.S
                ${TCC_DIR}/win32/lib/chkstk.S
                PROPERTIES LANGUAGE C
            )
        endif()

        # Platform definitions for runtime
        if(WIN32)
            target_compile_definitions(libtcc1_objs PRIVATE
                _WIN32=1
                TCC_TARGET_PE=1
            )
        else()
            target_compile_definitions(libtcc1_objs PRIVATE
                TCC_TARGET_ELF=1
            )
        endif()

        if(TCC_ARCH STREQUAL "x86_64")
            target_compile_definitions(libtcc1_objs PRIVATE TCC_TARGET_X86_64=1)
        else()
            target_compile_definitions(libtcc1_objs PRIVATE TCC_TARGET_I386=1)
        endif()

        add_library(libtcc1 STATIC $<TARGET_OBJECTS:libtcc1_objs>)
        set_target_properties(libtcc1 PROPERTIES
            OUTPUT_NAME "tcc1"
            ARCHIVE_OUTPUT_DIRECTORY ${TCC_OUTPUT_DIR}/lib
            PREFIX "lib"
        )

        add_custom_command(TARGET libtcc1 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE:libtcc1>
                ${TCC_OUTPUT_DIR}/lib/libtcc1.a
            COMMENT "Copying libtcc1.a to output directory"
        )
    else()
        message(WARNING "TCC: No libtcc1 sources found, runtime will not be built")
    endif()
endif()

# ============================================================
# Copy TCC Headers
# ============================================================
add_custom_target(tcc_headers ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TCC_DIR}/include
        ${TCC_OUTPUT_DIR}/include
    COMMENT "Copying TCC headers"
)

if(WIN32 AND EXISTS ${TCC_DIR}/win32/include)
    add_custom_command(TARGET tcc_headers POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${TCC_DIR}/win32/include
            ${TCC_OUTPUT_DIR}/include
        COMMENT "Copying Windows-specific TCC headers"
    )
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
    backend.cpp backend.h
    output_normalizer.h
    progressmanager.h progressmanager.cpp
)

# Enable TCC support
target_compile_definitions(SyntaxFlow PRIVATE
    SYNTAXFLOW_USE_TCC=1
)

# Link libraries (Qt BEFORE QScintilla)
target_link_libraries(SyntaxFlow PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    tree-sitter-highlighter
    libtcc
)

# Platform-specific linking
if(WIN32)
    # Windows/MinGW: Static linking
    if(MINGW)
        target_link_libraries(SyntaxFlow PRIVATE
            -static-libgcc
            -static-libstdc++
        )
    endif()
elseif(UNIX)
    # Linux: Need dl for dlopen/dlsym (used by TCC)
    target_link_libraries(SyntaxFlow PRIVATE
        dl
        pthread
    )
endif()

# ============================================================
# QScintilla
# ============================================================
target_include_directories(SyntaxFlow PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qscintilla/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qscintilla/scintilla/include
)

set(QSCI_LOCAL_WIN ${CMAKE_CURRENT_SOURCE_DIR}/libs/qscintilla/src/release/libqscintilla2_qt6.a)
set(QSCI_LOCAL_LINUX ${CMAKE_CURRENT_SOURCE_DIR}/libs/qscintilla/src/libqscintilla2.so)

if(WIN32 AND EXISTS ${QSCI_LOCAL_WIN})
    message(STATUS "QScintilla: Using local Windows build")
    target_link_libraries(SyntaxFlow PRIVATE ${QSCI_LOCAL_WIN})
elseif(UNIX AND EXISTS ${QSCI_LOCAL_LINUX})
    message(STATUS "QScintilla: Using local Linux build")
    target_link_libraries(SyntaxFlow PRIVATE ${QSCI_LOCAL_LINUX})
else()
    message(STATUS "QScintilla: Searching for system installation...")
    find_library(QSCI_SYSTEM_LIB
        NAMES qscintilla2_qt6 qscintilla2 libqscintilla2_qt6
        HINTS
            /usr/lib
            /usr/local/lib
            /usr/lib/${TCC_MULTIARCH}
    )
    if(QSCI_SYSTEM_LIB)
        message(STATUS "QScintilla: Found system library at ${QSCI_SYSTEM_LIB}")
        target_link_libraries(SyntaxFlow PRIVATE ${QSCI_SYSTEM_LIB})
    else()
        message(FATAL_ERROR "QScintilla not found! Please install it or build it locally.")
    endif()
endif()

# ============================================================
# Build Dependencies
# ============================================================
if(TARGET libtcc1)
    add_dependencies(SyntaxFlow libtcc1 tcc_headers)
else()
    add_dependencies(SyntaxFlow tcc_headers)
endif()

# ============================================================
# Post-Build: Copy Runtime Files
# ============================================================
add_custom_command(TARGET SyntaxFlow POST_BUILD
    # Create directories
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems"
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:SyntaxFlow>/tcc"
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/include"
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/lib"

    # Copy problems data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/problems"
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/problems.json"
        "$<TARGET_FILE_DIR:SyntaxFlow>/problems.json"

    # Copy TCC headers
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${TCC_OUTPUT_DIR}/include"
        "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/include"

    # Also copy from source as fallback
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${TCC_DIR}/include"
        "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/include"

    COMMENT "Copying runtime files to output directory"
)

# Copy libtcc1.a if it exists
if(TARGET libtcc1)
    add_custom_command(TARGET SyntaxFlow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${TCC_OUTPUT_DIR}/lib/libtcc1.a"
            "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/lib/libtcc1.a"
        COMMENT "Copying libtcc1.a"
    )
endif()

# Windows: Copy additional TCC files
if(WIN32)
    add_custom_command(TARGET SyntaxFlow POST_BUILD
        # Windows headers
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${TCC_DIR}/win32/include"
            "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/include"
        COMMENT "Copying Windows-specific TCC files"
    )

    # Windows lib files if they exist
    if(EXISTS ${TCC_DIR}/win32/lib)
        add_custom_command(TARGET SyntaxFlow POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${TCC_DIR}/win32/lib"
                "$<TARGET_FILE_DIR:SyntaxFlow>/tcc/lib"
            COMMENT "Copying Windows TCC libraries"
        )
    endif()
endif()

# ============================================================
# Windows: Hide Console & Static Linking
# ============================================================
if(WIN32)
    set_target_properties(SyntaxFlow PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# ============================================================
# Installation
# ============================================================
include(GNUInstallDirs)

install(TARGETS SyntaxFlow
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/problems
    DESTINATION ${CMAKE_INSTALL_DATADIR}/syntaxflow
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/problems.json
    DESTINATION ${CMAKE_INSTALL_DATADIR}/syntaxflow
)

install(DIRECTORY ${TCC_OUTPUT_DIR}/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/syntaxflow/tcc
)

# ============================================================
# Build Summary
# ============================================================
message(STATUS "")
message(STATUS "╔══════════════════════════════════════════╗")
message(STATUS "║            Build Summary                 ║")
message(STATUS "╠══════════════════════════════════════════╣")
message(STATUS "║  TCC Target:    ${TCC_PLATFORM} / ${TCC_ARCH}")
if(WIN32)
    message(STATUS "║  TCC Format:    PE + COFF")
else()
    message(STATUS "║  TCC Format:    ELF")
endif()
message(STATUS "║  TCC Output:    ${TCC_OUTPUT_DIR}")
message(STATUS "║  Build Type:    ${CMAKE_BUILD_TYPE}")
message(STATUS "╚══════════════════════════════════════════╝")
message(STATUS "")
