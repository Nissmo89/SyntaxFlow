cmake_minimum_required(VERSION 3.19)
project(SyntaxFlow LANGUAGES C CXX)

# ---- C++ & Qt Setup ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ============================================================
# Qt Prefix Hint & QWindowKit Options
# ============================================================
option(SYNTAXFLOW_USE_QWINDOWKIT "Enable QWindowKit frameless window integration" ON)
option(SYNTAXFLOW_REQUIRE_QWINDOWKIT "Fail configure if QWindowKit is not found when enabled" ON)
option(SYNTAXFLOW_FETCH_QWINDOWKIT "Fetch QWindowKit from source when not installed locally" ON)

set(SYNTAXFLOW_QT_PREFIX "" CACHE PATH "Preferred Qt prefix (example: /path/to/Qt/6.x/<toolchain>)")
set(SYNTAXFLOW_QWINDOWKIT_GIT_REPOSITORY "https://github.com/stdware/qwindowkit.git" CACHE STRING
    "Git repository used when SYNTAXFLOW_FETCH_QWINDOWKIT=ON")
set(SYNTAXFLOW_QWINDOWKIT_GIT_TAG "1.5.0" CACHE STRING
    "QWindowKit git tag/commit used when SYNTAXFLOW_FETCH_QWINDOWKIT=ON")

if(NOT SYNTAXFLOW_QT_PREFIX)
    foreach(_qt_env_var IN ITEMS QTDIR QT_DIR QT_ROOT)
        if(DEFINED ENV{${_qt_env_var}} AND EXISTS "$ENV{${_qt_env_var}}")
            set(SYNTAXFLOW_QT_PREFIX "$ENV{${_qt_env_var}}")
            break()
        endif()
    endforeach()
endif()

if(NOT SYNTAXFLOW_QT_PREFIX)
    file(GLOB _NORD_HOME_QT_PREFIXES LIST_DIRECTORIES true "$ENV{HOME}/Qt/*/gcc_64")
    if(_NORD_HOME_QT_PREFIXES)
        list(SORT _NORD_HOME_QT_PREFIXES COMPARE NATURAL)
        list(GET _NORD_HOME_QT_PREFIXES -1 SYNTAXFLOW_QT_PREFIX)
    endif()
endif()

if(SYNTAXFLOW_QT_PREFIX)
    message(STATUS "Using Qt prefix: ${SYNTAXFLOW_QT_PREFIX}")
    list(PREPEND CMAKE_PREFIX_PATH "${SYNTAXFLOW_QT_PREFIX}")
endif()

# ---- Find Qt ----
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network WebEngineWidgets WebChannel Svg)

set(SYNTAXFLOW_HAS_QWINDOWKIT OFF)
set(SYNTAXFLOW_QWINDOWKIT_SOURCE "disabled")
if(SYNTAXFLOW_USE_QWINDOWKIT)
    if(NOT QWindowKit_DIR AND SYNTAXFLOW_QT_PREFIX AND EXISTS "${SYNTAXFLOW_QT_PREFIX}/lib/cmake/QWindowKit")
        set(QWindowKit_DIR "${SYNTAXFLOW_QT_PREFIX}/lib/cmake/QWindowKit" CACHE PATH "Path to QWindowKitConfig.cmake" FORCE)
    endif()

    find_package(QWindowKit QUIET CONFIG COMPONENTS Widgets)
    if(QWindowKit_FOUND AND TARGET QWindowKit::Widgets)
        set(SYNTAXFLOW_HAS_QWINDOWKIT ON)
        set(SYNTAXFLOW_QWINDOWKIT_SOURCE "system")
    endif()

    if(NOT SYNTAXFLOW_HAS_QWINDOWKIT)
        foreach(_SYNTAXFLOW_QWK_CHECK_DIR IN ITEMS "${CMAKE_SOURCE_DIR}/external/qwindowkit" "${CMAKE_SOURCE_DIR}/libs/qwindowkit")
            if(EXISTS "${_SYNTAXFLOW_QWK_CHECK_DIR}/CMakeLists.txt")
                message(STATUS "Using vendored QWindowKit sources at: ${_SYNTAXFLOW_QWK_CHECK_DIR}")
                set(QWINDOWKIT_BUILD_WIDGETS ON CACHE BOOL "Build QWindowKit widgets module" FORCE)
                set(QWINDOWKIT_BUILD_QUICK OFF CACHE BOOL "Build QWindowKit quick module" FORCE)
                set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "Build QWindowKit examples" FORCE)
                set(QWINDOWKIT_BUILD_DOCUMENTATIONS OFF CACHE BOOL "Build QWindowKit docs" FORCE)
                set(QWINDOWKIT_INSTALL OFF CACHE BOOL "Disable QWindowKit install rules in SyntaxFlow superbuild" FORCE)
                add_subdirectory("${_SYNTAXFLOW_QWK_CHECK_DIR}" "${CMAKE_BINARY_DIR}/_deps/qwindowkit-build")
                if(TARGET QWindowKit::Widgets)
                    set(SYNTAXFLOW_HAS_QWINDOWKIT ON)
                    set(SYNTAXFLOW_QWINDOWKIT_SOURCE "vendored")
                    break()
                endif()
            endif()
        endforeach()
    endif()

    if(NOT SYNTAXFLOW_HAS_QWINDOWKIT AND SYNTAXFLOW_FETCH_QWINDOWKIT)
        include(FetchContent)
        message(STATUS
            "QWindowKit not found locally. Fetching ${SYNTAXFLOW_QWINDOWKIT_GIT_REPOSITORY} "
            "(tag: ${SYNTAXFLOW_QWINDOWKIT_GIT_TAG})"
        )
        set(QWINDOWKIT_BUILD_WIDGETS ON CACHE BOOL "Build QWindowKit widgets module" FORCE)
        set(QWINDOWKIT_BUILD_QUICK OFF CACHE BOOL "Build QWindowKit quick module" FORCE)
        set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "Build QWindowKit examples" FORCE)
        set(QWINDOWKIT_BUILD_DOCUMENTATIONS OFF CACHE BOOL "Build QWindowKit docs" FORCE)
        set(QWINDOWKIT_INSTALL OFF CACHE BOOL "Disable QWindowKit install rules in SyntaxFlow superbuild" FORCE)
        FetchContent_Declare(qwindowkit
            GIT_REPOSITORY "${SYNTAXFLOW_QWINDOWKIT_GIT_REPOSITORY}"
            GIT_TAG "${SYNTAXFLOW_QWINDOWKIT_GIT_TAG}"
            GIT_SHALLOW TRUE
            GIT_SUBMODULES "qmsetup"
            GIT_SUBMODULES_RECURSE TRUE
        )
        FetchContent_MakeAvailable(qwindowkit)
        if(TARGET QWindowKit::Widgets)
            set(SYNTAXFLOW_HAS_QWINDOWKIT ON)
            set(SYNTAXFLOW_QWINDOWKIT_SOURCE "fetched")
        endif()
    endif()

    if(SYNTAXFLOW_HAS_QWINDOWKIT)
        message(STATUS "QWindowKit enabled (source: ${SYNTAXFLOW_QWINDOWKIT_SOURCE}).")
    elseif(SYNTAXFLOW_REQUIRE_QWINDOWKIT)
        message(FATAL_ERROR "QWindowKit is required but unavailable.")
    else()
        message(WARNING "QWindowKit not found. Building without frameless integration.")
    endif()
endif()

# ============================================================
# QText Browser Custom Renderer Module
# ============================================================
add_subdirectory(libs/qtext_browser)



# ============================================================
# Wasmer Offline Pre-caching
# ============================================================
option(PRECACHE_WASM_ENGINES "Download and cache Wasmer engines (Python, Clang, QuickJS) during build for offline use" OFF)

if(PRECACHE_WASM_ENGINES)
    message(STATUS "Enabled pre-caching of Wasmer engines. This will occur during build.")
    
    if(WIN32)
        set(WASMER_CMD "${CMAKE_CURRENT_SOURCE_DIR}/tools/wasmer/bin/wasmer.exe")
    else()
        set(WASMER_CMD "${CMAKE_CURRENT_SOURCE_DIR}/tools/wasmer/bin/wasmer")
    endif()

    # Create a custom target that runs before SyntaxFlow
    add_custom_target(PrecacheWasmEngines ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Using locally bundled Wasmer engines Python and QuickJS..."
        COMMENT "Pre-caching language engines via Wasmer..."
    )
    
    # ── Fetch WASI-SDK for instant C++ compilation ──
    include(FetchContent)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(WASI_SDK_URL "https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/wasi-sdk-20.0-linux.tar.gz")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(WASI_SDK_URL "https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/wasi-sdk-20.0-macos.tar.gz")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(WASI_SDK_URL "https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/wasi-sdk-20.0.m-mingw.tar.gz")
    endif()

    if(WASI_SDK_URL)
        message(STATUS "Fetching native wasi-sdk for instant C++ compilation...")
        FetchContent_Declare(
          wasi_sdk
          URL ${WASI_SDK_URL}
        )
        FetchContent_MakeAvailable(wasi_sdk)
    endif()
else()
    message(STATUS "=====================================================================================")
    message(STATUS "NOTE: Wasmer language engines (Python, Clang, JS) will be downloaded on FIRST RUN.")
    message(STATUS "      To pre-download them at build time (for offline use), configure CMake with:")
    message(STATUS "      -DPRECACHE_WASM_ENGINES=ON")
    message(STATUS "=====================================================================================")
endif()

# ============================================================
# Main Executable
# ============================================================
add_executable(SyntaxFlow
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
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
    javascript_runner.h javascript_runner.cpp
    wasm_runner.h wasm_runner.cpp
    httpserver.h
    resources/resources.qrc
)

# target_include_directories(SyntaxFlow PRIVATE ${Python3_INCLUDE_DIRS})

# ---- Link Libraries ----
target_link_libraries(SyntaxFlow PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::WebEngineWidgets
    Qt6::WebChannel
    Qt6::Svg
    qtext_browser
)

if(SYNTAXFLOW_HAS_QWINDOWKIT)
    target_link_libraries(SyntaxFlow PRIVATE QWindowKit::Widgets)
    target_compile_definitions(SyntaxFlow PRIVATE SYNTAXFLOW_HAS_QWINDOWKIT)
endif()

if(PRECACHE_WASM_ENGINES)
    add_dependencies(SyntaxFlow PrecacheWasmEngines)
endif()

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

if(PRECACHE_WASM_ENGINES AND wasi_sdk_SOURCE_DIR)
    add_custom_command(TARGET SyntaxFlow POST_BUILD
        COMMENT "Copying wasi-sdk runtime..."
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${wasi_sdk_SOURCE_DIR}"
            "$<TARGET_FILE_DIR:SyntaxFlow>/wasi-sdk"
    )
endif()


# ---- Windows: hide console ----
if(WIN32)
    set_target_properties(SyntaxFlow PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
