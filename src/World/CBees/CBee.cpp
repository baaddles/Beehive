#include "World/CBees/CBee.hpp"

CBee::CBee(sf::Vector2f pos, EWindowType win, float speed)
    : m_position(pos), m_windowType(win), m_speed(speed)
{
    m_sprite.setTexture(
        CTextureManager::instance().getTexture("default.png")
    );
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

void CBee::keepInsideWindow(const sf::Vector2u& windowSize)
{
    if (m_position.x < 0) m_position.x = 0;
    if (m_position.y < 0) m_position.y = 0;
    if (m_position.x > windowSize.x - m_sprite.getGlobalBounds().width)
        m_position.x = windowSize.x - m_sprite.getGlobalBounds().width;
    if (m_position.y > windowSize.y - m_sprite.getGlobalBounds().height)
        m_position.y = windowSize.y - m_sprite.getGlobalBounds().height;

    m_sprite.setPosition(m_position);
}
