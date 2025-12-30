#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "World/CScene.hpp"
#include "World/COutsideScene.hpp"
#include "World/CHiveScene.hpp"

class CApplication
{
private:
    sf::RenderWindow m_window;
    std::unique_ptr<CScene> m_currentScene;

    void handleResize(const sf::Event& event);
    void handleEvents();
    void update(float dt);
    void render();

    void switchScene();

public:
    CApplication();
    void run();
};
