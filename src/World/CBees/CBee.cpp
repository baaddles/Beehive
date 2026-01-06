#include "World/CBees/CBee.hpp"

CBee::CBee(sf::Vector2f pos, EWindowType win, float speed)
    : m_position(pos), m_windowType(win), m_speed(speed)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("default.png"), true);
    
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    m_sprite.setPosition(m_position);
}

void CBee::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}

sf::Vector2f CBee::getPosition() const
{
    return m_position;
}

void CBee::setPosition(const sf::Vector2f& pos)
{
    m_position = pos;
    m_sprite.setPosition(m_position);
}

void CBee::setWindowType(EWindowType newWindow)
{
    m_windowType = newWindow;
}

EWindowType CBee::getWindowType() const
{
    return m_windowType;
}

sf::FloatRect CBee::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

void CBee::keepInsideWindow(const sf::Vector2u& windowSize)
{
    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    float halfWidth = bounds.width / 2.f;
    float halfHeight = bounds.height / 2.f;

    if (m_position.x < halfWidth) m_position.x = halfWidth;
    if (m_position.y < halfHeight) m_position.y = halfHeight;
    
    if (m_position.x > windowSize.x - halfWidth)
        m_position.x = windowSize.x - halfWidth;
    if (m_position.y > windowSize.y - halfHeight)
        m_position.y = windowSize.y - halfHeight;

    m_sprite.setPosition(m_position);
}