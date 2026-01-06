cmake_minimum_required(VERSION 3.10)

project(Beehive VERSION 0.1.0 LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

# SFML
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# Sources
file(GLOB_RECURSE PROJECT_SOURCES
    src/*.cpp
    main.cpp
)

add_executable(Beehive
    ${PROJECT_SOURCES}
)

target_include_directories(Beehive
    PRIVATE
        include
)

target_link_libraries(Beehive
    sfml-graphics
    sfml-window
    sfml-system
)

# --- NOUVEAU : GESTION DES ASSETS ---
# Cette commande copie le dossier assets de la RACINE vers le dossier BUILD
# à chaque fois que tu lances CMake.
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")