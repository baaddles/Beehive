#pragma once
#include <SFML/Graphics.hpp>
#include "Core/CSimulation.hpp"

class CApplication
{
private:
    sf::RenderWindow m_window;
    CSimulation m_simulation;

    void handleEvents();
    void render();

public:
    CApplication();
    void run();
};
