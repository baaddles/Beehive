#pragma once
#include <SFML/Graphics.hpp>
#include "Utils/CEnums.hpp"
#include <string>

class CEntity
{
protected:
    sf::Vector2f m_position;
    EWindowType m_windowType;

public:
    // Constructeur (Déclaration seule)
    CEntity(sf::Vector2f pos, EWindowType windowType);
    virtual ~CEntity() = default;

    // Méthodes virtuelles pures
    virtual void update(float dt, const sf::Vector2u& windowSize) = 0; 
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual sf::FloatRect getBounds() const = 0; 

    // Méthode virtuelle (Déclaration seule)
    virtual std::string getStats() const;

    // Getters / Setters (Déclarations seules)
    EWindowType getWindowType() const;
    void setWindowType(EWindowType type);
    sf::Vector2f getPosition() const;
    virtual void setPosition(sf::Vector2f pos);
};