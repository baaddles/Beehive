#include "World/CBees/CWarriorBee.hpp"

CWarriorBee::CWarriorBee(sf::Vector2f pos)
    : CBee(pos, EWindowType::BEEHIVE), m_state(EWarriorState::REPOS), m_force(10), m_attackFrequency(1.0f)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("warrior_bee.png"));
}

void CWarriorBee::update(float dt, const sf::Vector2u& windowSize)
{
    // Logique de patrouille simple
    keepInsideWindow(windowSize);
}

void CWarriorBee::draw(sf::RenderWindow& window) const
{
    CBee::draw(window);
}