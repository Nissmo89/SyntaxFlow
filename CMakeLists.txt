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
    qtext_browser
)

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
