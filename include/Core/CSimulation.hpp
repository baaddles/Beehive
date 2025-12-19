#pragma once
#include <vector>
#include <memory>
#include "World/CEntity.hpp"
#include "World/CBeehive.hpp"

class CSimulation
{
private:
    std::vector<std::unique_ptr<CEntity>> m_entities; // entités dans la simulation
    CBeehive m_beehive; // la ruche

public:
    CSimulation();

    void update(float dt);
    void draw(sf::RenderWindow& hiveWindow, sf::RenderWindow& outsideWindow); // dessiner les fenêtres
};