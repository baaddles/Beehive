#pragma once

#include "World/CScene.hpp"

class CHiveScene : public CScene
{
public:
    CHiveScene();

    void handleEvent(
        const sf::Event& event,
        const sf::RenderWindow& window,
        bool& requestSceneSwitch
    ) override;

    void update(float dt) override;

    void draw(sf::RenderWindow& window) const override;

    sf::FloatRect getActionZone(
        const sf::Vector2u& windowSize
    ) const override;
};
