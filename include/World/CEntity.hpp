#pragma once
#include <SFML/Graphics.hpp>
#include "Utils/CEnums.hpp"

class CEntity
{
protected:
    sf::Vector2f m_position;
    EWindowType m_windowType;

public:
    CEntity(sf::Vector2f pos, EWindowType windowType);
    virtual ~CEntity() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    EWindowType getWindowType() const;
};
