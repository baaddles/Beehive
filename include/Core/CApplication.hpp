#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "World/CBees/CBee.hpp"
#include "World/CBees/CWorkerBee.hpp"
#include "World/CBees/CQueenBee.hpp"
#include "World/CBees/CWarriorBee.hpp"
#include "World/CFlower.hpp"
#include "Graphics/CTextureManager.hpp"
#include "World/CIntruder.hpp"

class CApplication
{
public:
    CApplication();
    void run();

private:
    void handleEvents();
    void update(float dt);
    void render();
    void addPollenToHive(int amount);
    void performLevelUp();

    int m_hiveLevel;
    int m_currentPollen;      // Experience actuelle
    int m_pollenForNextLevel; // Objectif à atteindre

private:
    sf::RenderWindow m_outsideWindow;
    sf::RenderWindow m_insideWindow;

    sf::Sprite m_outsideBackground;
    sf::Sprite m_insideBackground;
    
    // Rectangle pour le sol vert
    sf::RectangleShape m_grassRect;

    std::vector<std::unique_ptr<CBee>> m_entities;
    std::vector<std::unique_ptr<CFlower>> m_flowers;
    std::vector<std::unique_ptr<CIntruder>> m_intruders;
};