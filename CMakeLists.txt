cmake_minimum_required(VERSION 3.10)
project(Beehive VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp main.cpp)

add_executable(Beehive ${PROJECT_SOURCES})

target_compile_definitions(Beehive PRIVATE ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/")
target_compile_definitions(Beehive PRIVATE FONTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts/")

target_include_directories(Beehive PRIVATE include)
target_link_libraries(Beehive sfml-graphics sfml-window sfml-system)