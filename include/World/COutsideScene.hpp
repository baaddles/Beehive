#pragma once
#include <memory>
#include <vector>
#include "World/CScene.hpp"
#include "World/CEntity.hpp"



class COutsideScene : public CScene
{
private:
    std::vector<std::unique_ptr<CEntity>> m_entities;
public:
    COutsideScene();

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
