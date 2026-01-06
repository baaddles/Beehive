#pragma once
#include <SFML/Graphics.hpp>

namespace constants {
    const float FLOWER_REGEN_TIME = 30.0f; 
    const int POLLEN_PER_FLOWER = 3;
    const int INITIAL_FLOWER_COUNT = 20; 
    
    const float GRASS_HEIGHT_RATIO = 0.66f;
    const sf::Color GRASS_COLOR = sf::Color(34, 139, 34);
    
    const int WORKER_CAPACITY = 3;

    // Vitesse réduite (était à 120.f)
    const float WORKER_SPEED = 70.f;

    const float HIVE_CENTER_X = 350.f;
    const float HIVE_CENTER_Y = 250.f;
}