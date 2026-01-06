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

public:
    CBee(sf::Vector2f pos, EWindowType win, float speed = 50.f);
    virtual ~CBee() = default;

    virtual void update(float dt, const sf::Vector2u& windowSize) = 0;
    virtual void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);

    void setWindowType(EWindowType newWindow);
    EWindowType getWindowType() const;

protected:
    void keepInsideWindow(const sf::Vector2u& windowSize);
};