#pragma once

namespace constants {
    // Temps de régénération d'une fleur (60 secondes)
    const float FLOWER_REGEN_TIME = 60.0f; 
    
    // Quantité de pollen par fleur
    const int POLLEN_PER_FLOWER = 3;
    
    // Nombre initial de fleurs
    const int INITIAL_FLOWER_COUNT = 10;
    
    // Zone d'herbe : 33% du bas de l'écran
    const float GRASS_HEIGHT_RATIO = 0.33f;
    
    // Capacité de transport d'une ouvrière
    const int WORKER_CAPACITY = 3;
}