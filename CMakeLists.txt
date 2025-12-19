cmake_minimum_required(VERSION 3.10)

# Nom du projet et langage utilisé
project(Beehive VERSION 0.1.0 LANGUAGES CXX)

# ---------------------------------------
# Configuration générale du compilateur
# ---------------------------------------

# Standard C++ imposé
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Activer les warnings (très utile pour un projet pédagogique)
add_compile_options(-Wall -Wextra -Wpedantic)

# ---------------------------------------
# Détection de SFML
# ---------------------------------------

# Recherche des modules SFML nécessaires
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# ---------------------------------------
# Fichiers sources du projet
# ---------------------------------------

# Récupère automatiquement TOUS les .cpp dans src/
file(GLOB_RECURSE PROJECT_SOURCES
    src/*.cpp
    main.cpp
)

# ---------------------------------------
# Création de l'exécutable
# ---------------------------------------

add_executable(Beehive
    ${PROJECT_SOURCES}
)

# ---------------------------------------
# Include directories
# ---------------------------------------

# Permet d'inclure avec : #include "Core/CSimulation.hpp"
target_include_directories(Beehive
    PRIVATE
        include
)

# ---------------------------------------
# Link avec SFML
# ---------------------------------------

target_link_libraries(Beehive
    sfml-graphics
    sfml-window
    sfml-system
)
