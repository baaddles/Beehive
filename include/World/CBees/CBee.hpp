#pragma once
#include <SFML/Graphics.hpp>
#include "../../Utils/CEnums.hpp"
#include "../../Graphics/CTextureManager.hpp"

class CBee
{
protected:
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    EWindowType m_windowType;
    float m_speed;
    sf::Vector2f m_homePosition;

public:
    CBee(sf::Vector2f pos, EWindowType win, float speed = 50.f);
    virtual ~CBee() = default;

    virtual void update(float dt, const sf::Vector2u& windowSize) = 0;
    virtual void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);

    void setWindowType(EWindowType newWindow);
    EWindowType getWindowType() const;

    void setHomePosition(const sf::Vector2f& home) { m_homePosition = home; }
    sf::Vector2f getHomePosition() const { return m_homePosition; }
    float getSpeed() const { return m_speed; }

    // Utile pour les collisions avec les fleurs
    sf::FloatRect getBounds() const;

    virtual void levelUpStats(float factor) = 0;

protected:
    void keepInsideWindow(const sf::Vector2u& windowSize);
};