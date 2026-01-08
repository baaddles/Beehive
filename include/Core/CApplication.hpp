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
    
    // UI Render Function
    void renderUI(sf::RenderWindow& window);
    void initUI();

    void addPollenToHive(int amount);
    void performLevelUp();
    void spawnWave();

    int m_hiveLevel;
    int m_currentPollen;
    int m_pollenForNextLevel;

    int m_hiveHealth;
    int m_hiveMaxHealth;
    int m_waveCount;

    // --- UI ELEMENTS ---
    sf::Font m_font;
    sf::Text m_txtCounters;
    sf::Text m_txtLevelInfo;
    sf::Text m_txtGameOver;
    sf::Text m_txtHealthInfo;

    // Health Bar
    sf::RectangleShape m_barHealthBack;
    sf::RectangleShape m_barHealthFront;

    // XP Bar
    sf::RectangleShape m_barXPBack;
    sf::RectangleShape m_barXPFront;

private:
    sf::RenderWindow m_outsideWindow;
    sf::RenderWindow m_insideWindow;

    sf::Sprite m_outsideBackground;
    sf::Sprite m_insideBackground;
    sf::RectangleShape m_grassRect;

    std::vector<std::unique_ptr<CBee>> m_entities;
    std::vector<std::unique_ptr<CFlower>> m_flowers;
    std::vector<std::unique_ptr<CIntruder>> m_intruders;
};