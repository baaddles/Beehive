#include "World/CBees/CWarriorBee.hpp"
#include <cmath>
#include <iostream>

CWarriorBee::CWarriorBee(sf::Vector2f pos, EWindowType win)
    : CBee(pos, EWindowType::BEEHIVE), m_state(EWarriorState::REPOS), m_force(2), m_attackFrequency(1.0f), m_attackTimer(0.f)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("warrior_bee.png"));
}

void CWarriorBee::update(float dt, const sf::Vector2u& windowSize)
{
    if (m_attackTimer > 0.f) {
        m_attackTimer -= dt;
    }

    sf::Vector2f target = m_homePosition;
    sf::Vector2f diff = target - m_position;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist > 3.f) {
        m_position += (diff / dist) * m_speed * dt;
    }

    m_sprite.setPosition(m_position);
    // On garde l'abeille dans les limites de la fenêtre
    keepInsideWindow(windowSize);
}

void CWarriorBee::draw(sf::RenderWindow& window) const
{
    CBee::draw(window);
}

void CWarriorBee::levelUpStats(float factor) {
    // Augmente la vitesse
    m_speed *= (1.0f + factor);
    
    // Augmente la force d'attaque
    m_force += 2; 
    
    std::cout << "Warrior est plus forte ! Vitesse: " << m_speed << ", Force: " << m_force << std::endl;
}