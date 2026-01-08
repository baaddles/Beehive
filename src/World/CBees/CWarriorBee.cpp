#include "World/CBees/CWarriorBee.hpp"
#include "Utils/CConstants.hpp"
#include <cmath>
#include <iostream>

CWarriorBee::CWarriorBee(sf::Vector2f pos, EWindowType win)
    : CBee(pos, win, constants::WARRIOR_SPEED), 
      m_state(EWarriorState::REPOS), 
      m_force(10), 
      m_attackFrequency(1.0f), 
      m_attackTimer(0.f),
      m_recoilVelocity(0.f, 0.f)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("warrior_bee.png"));
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void CWarriorBee::applyKnockback(sf::Vector2f direction, float force) {
    // On normalise la direction et on applique la force
    float len = std::sqrt(direction.x*direction.x + direction.y*direction.y);
    if (len != 0) {
        m_recoilVelocity = (direction / len) * force;
    }
}

void CWarriorBee::update(float dt, const sf::Vector2u& windowSize)
{
    // 1. Timer Attaque
    if (m_attackTimer > 0.f) m_attackTimer -= dt;

    // 2. Gestion du Recul Fluide (Inertie)
    if (std::abs(m_recoilVelocity.x) > 1.f || std::abs(m_recoilVelocity.y) > 1.f) {
        m_position += m_recoilVelocity * dt;
        // Friction : on réduit la vitesse de recul à chaque frame (ex: 90% restant)
        m_recoilVelocity *= 0.90f; 
    }
    else {
        // 3. Mouvement Normal (Seulement si le recul est fini ou très faible)
        m_recoilVelocity = sf::Vector2f(0.f, 0.f); // Stop total
        
        sf::Vector2f diff = m_homePosition - m_position;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist > 3.f) {
            sf::Vector2f dir = diff / dist;
            m_position += dir * m_speed * dt;
            
            // --- FLUIDITÉ & ORIENTATION ---
            // On ne tourne plus (setRotation). On Flip horizontalement.
            // Si va à gauche (< -0.1), scale X = -1
            // Si va à droite (> 0.1), scale X = 1
            if (dir.x < -0.1f) m_sprite.setScale(-1.f, 1.f);
            else if (dir.x > 0.1f) m_sprite.setScale(1.f, 1.f);
        }
    }

    // Pas de rotation verticale (on garde 0 par défaut ou on reset)
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
    m_force += 2; 
    std::cout << "Warrior UP! Speed: " << m_speed << ", Force: " << m_force << std::endl;
}