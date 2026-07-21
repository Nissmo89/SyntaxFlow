cmake_minimum_required(VERSION 3.19)
project(qtext_browser LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

add_library(qtext_browser STATIC
    networkbrowser.cpp
    networkbrowser.h
)

target_include_directories(qtext_browser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qtext_browser PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)
