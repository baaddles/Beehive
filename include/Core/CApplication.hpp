#pragma once
#include <SFML/Graphics.hpp>
#include "Core/CSimulation.hpp"

class CApplication
{
    private:
        sf::RenderWindow m_hiveWindow; // fenêtre ruche
        sf::RenderWindow m_outsideWindow; // fenêtre extérieur
        CSimulation m_simulation; // simulation entière contenant les entités et la ruche

    public:
        CApplication();
        void run();
};