#pragma once
#include <SFML/Graphics.hpp>
#include "../Utils/CEnums.hpp"

class CEntity
{
protected:
    sf::Vector2f m_position; // Coordonnées XY sur la fenêtre d'une entité
    EWindowType m_windowType; // Fenêtre dans laquelle l'entité se situe

public:
    CEntity(sf::Vector2f pos, EWindowType win);
    virtual ~CEntity() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    EWindowType getWindowType() const;
};