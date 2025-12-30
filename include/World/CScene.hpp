#pragma once
#include <SFML/Graphics.hpp>
#include "Utils/CEnums.hpp"

class CScene
{
protected:
    EWindowType m_type;
public:
    explicit CScene(EWindowType type);
    virtual ~CScene() = default;
    
    virtual void handleEvent(
        const sf::Event& event,
        const sf::RenderWindow& window,
        bool& requestSceneSwitch
    ) = 0;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    EWindowType getType() const;

    virtual sf::FloatRect getActionZone(
        const sf::Vector2u& windowSize
    ) const = 0;
};