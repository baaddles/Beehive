#pragma once
#include <SFML/Graphics.hpp>

namespace constants {
    
    // --- FENÊTRES ---
    const int WINDOW_WIDTH = 700;
    const int WINDOW_HEIGHT = 500;
    const int WINDOW_POS_X = 100;
    const int WINDOW_POS_Y = 100;
    const int WINDOW_OFFSET = 720; // Décalage entre les deux fenêtres

    // --- ENVIRONNEMENT ---
    const float FLOWER_REGEN_TIME = 15.0f; 
    const int POLLEN_PER_FLOWER = 3;
    const int INITIAL_FLOWER_COUNT = 25; 
    const float GRASS_HEIGHT_RATIO = 0.66f;
    const sf::Color GRASS_COLOR = sf::Color(34, 139, 34);

    // --- RUCHE & GAMEPLAY ---
    const int INITIAL_HIVE_HEALTH = 100;
    const int HEALTH_GAIN_ON_LEVEL_UP = 20;
    const int POLLEN_FOR_FIRST_LEVEL = 50;
    const int HEAL_PER_DELIVERY = 1; // Soin fixe par livraison
    const float LEVEL_UP_STAT_BUFF = 0.1f; // +10% stats
    const float LEVEL_UP_HEAL_RATIO = 0.10f; // 10% soin au level up

    const float HIVE_CENTER_X = 350.f;
    const float HIVE_CENTER_Y = 250.f;

    // --- ABEILLES ---
    const int INITIAL_WORKER_COUNT = 5;
    const int INITIAL_WARRIOR_COUNT = 5;
    
    const int WORKER_CAPACITY = 6;
    const float WORKER_SPEED = 150.f; 
    const float WARRIOR_SPEED = 150.f;

    // --- GRILLE GUERRIÈRES (POSITIONNEMENT) ---
    const int WARRIORS_PER_COLUMN = 5; // Combien avant de changer de colonne
    const float GRID_START_X = 450.f;
    const float GRID_START_Y = 100.f;
    const float GRID_SPACING_X = 50.f;
    const float GRID_SPACING_Y = 60.f;

    // --- INTRUS & VAGUES ---
    const int INTRUDERS_PER_WAVE = 3;
    const int BASE_INTRUDER_HP = 250;
    const int BASE_INTRUDER_FORCE = 15;
    const float BASE_INTRUDER_SPEED = 50.0f;
    
    // Multiplicateurs de difficulté
    const float WAVE_HP_FORCE_MULT = 0.2f; // +20% par manche
    const float WAVE_SPEED_MULT = 0.05f;   // +5% par manche

    // --- UI (INTERFACE) ---
    const float UI_BAR_WIDTH = 300.f;
    const float UI_HEALTH_BAR_HEIGHT = 24.f;
    const float UI_XP_BAR_HEIGHT = 16.f;
    
    // Position des barres (Centre X = 350)
    const float UI_HEALTH_Y = 420.f;
    const float UI_XP_Y = 455.f;
}