#include "World/CBees/CWarriorBee.hpp"
#include "Graphics/CTextureManager.hpp"
#include "Utils/CConstants.hpp"
#include <cmath>
#include <iostream>

CWarriorBee::CWarriorBee(sf::Vector2f pos, EWindowType win)
    : CBee(pos, win, constants::WARRIOR_SPEED), 
      m_state(EWarriorState::REPOS),
      m_force(26 + (std::rand() % 7)), 
      m_attackFrequency(1.0f), 
      m_attackTimer(0.f),
      m_recoilVelocity(0.f, 0.f)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("warrior_bee.png"));
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void CWarriorBee::applyKnockback(sf::Vector2f direction, float force) {
    float len = std::sqrt(direction.x*direction.x + direction.y*direction.y);
    if (len != 0) {
        m_recoilVelocity = (direction / len) * force;
    }
}

void CWarriorBee::update(float dt, const sf::Vector2u& windowSize)
{
    if (m_attackTimer > 0.f) m_attackTimer -= dt;

    // Gestion du recul
    if (std::abs(m_recoilVelocity.x) > 1.f || std::abs(m_recoilVelocity.y) > 1.f) {
        m_position += m_recoilVelocity * dt;
        m_recoilVelocity *= 0.90f; 
    }
    else {
        m_recoilVelocity = sf::Vector2f(0.f, 0.f);
        
        sf::Vector2f diff = m_homePosition - m_position;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist > 3.f) {
            sf::Vector2f dir = diff / dist;
            m_position += dir * m_speed * dt;
            
            // Flip horizontal selon la direction
            if (dir.x < -0.1f) m_sprite.setScale(-1.f, 1.f);
            else if (dir.x > 0.1f) m_sprite.setScale(1.f, 1.f);
        } else {
            // --- MODIFICATION : ORIENTATION AU REPOS ---
            // Quand elle est arrivée à sa position de garde, elle regarde vers la droite (l'extérieur)
            m_sprite.setScale(1.f, 1.f);
        }
    }

    m_sprite.setRotation(0.f); 
    m_sprite.setPosition(m_position);
    keepInsideWindow(windowSize);
}

void CWarriorBee::draw(sf::RenderWindow& window) const {
    CBee::draw(window);
}

void CWarriorBee::resetAttackTimer() {
    m_attackTimer = m_attackFrequency;
}

void CWarriorBee::levelUpStats(float factor) {
    m_speed *= (1.0f + factor);
    // --- MODIFICATION : Gain de force réduit ---
    m_force += 1;  // +1 au lieu de +2
    std::cout << "Warrior UP! Speed: " << m_speed << ", Force: " << m_force << std::endl;
}