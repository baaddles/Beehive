#include "World/CBees/CQueenBee.hpp"
#include "Graphics/CTextureManager.hpp"

CQueenBee::CQueenBee(sf::Vector2f pos)
    : CBee(pos, EWindowType::BEEHIVE, 2), // type INSIDE pour reine
      m_reproduction(false),
      m_reproductionFrequency(5.f),
      m_verticalDirection(1.f)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("queen_bee.png"));
}

void CQueenBee::update(float dt, const sf::Vector2u& windowSize)
{
    sf::Vector2f pos = getPosition();
    pos.y += 30.f * dt * m_verticalDirection;
    if (pos.y < 50.f || pos.y > windowSize.y - 50.f)
        m_verticalDirection *= -1.f;

    setPosition(pos);
    keepInsideWindow(windowSize);
}

void CQueenBee::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}
