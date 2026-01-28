cmake_minimum_required(VERSION 3.19)
project(SyntaxFlow LANGUAGES CXX)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Widgets WebEngineWidgets WebChannel)

qt_standard_project_setup()

# Copy data directory to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


qt_add_executable(SyntaxFlow
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    javabridge.h javabridge.cpp
    backend.h backend.cpp
    judgebridge.h judgebridge.cpp
)

target_link_libraries(SyntaxFlow
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Gui
        Qt6::WebEngineWidgets
        Qt6::WebChannel
)

include(GNUInstallDirs)

install(TARGETS SyntaxFlow
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# qt_generate_deploy_app_script(
#     TARGET Code_hour
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )
# install(SCRIPT ${deploy_script})
