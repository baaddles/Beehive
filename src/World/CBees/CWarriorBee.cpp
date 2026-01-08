#include "World/CBees/CWarriorBee.hpp"
#include "Graphics/CTextureManager.hpp"
#include "Utils/CConstants.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <cstdlib>

CWarriorBee::CWarriorBee(sf::Vector2f pos, EWindowType win)
    : CBee(pos, win, 0.f), // On initialise speed à 0 ici, on le calcule juste après
      m_state(EWarriorState::REPOS),
      m_force(4 + (std::rand() % 3)), 
      m_attackFrequency(1.5f), 
      m_attackTimer(0.f),
      m_recoilVelocity(0.f, 0.f)
{
    // --- RANDOMISATION VITESSE (Biaisée vers le bas) ---
    // On génère un float entre 0.0 et 1.0
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    // On met r au carré (r*r). 
    // Ex: si r=0.5 -> 0.25. Si r=0.9 -> 0.81.
    // Cela augmente la probabilité d'avoir des petits nombres, donc proche du MIN.
    float biasedR = r * r; 

    m_speed = constants::WARRIOR_MIN_SPEED + biasedR * (constants::WARRIOR_MAX_SPEED - constants::WARRIOR_MIN_SPEED);

    // ... (Sprite init inchangé) ...
    m_sprite.setTexture(CTextureManager::instance().getTexture("warrior_bee.png"));
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void CWarriorBee::applyKnockback(sf::Vector2f direction, float force) {
    float len = std::sqrt(direction.x*direction.x + direction.y*direction.y);
    if (len != 0) m_recoilVelocity = (direction / len) * force;
}

void CWarriorBee::update(float dt, const sf::Vector2u& windowSize) {
    if (m_attackTimer > 0.f) m_attackTimer -= dt;

    if (std::abs(m_recoilVelocity.x) > 1.f || std::abs(m_recoilVelocity.y) > 1.f) {
        m_position += m_recoilVelocity * dt;
        m_recoilVelocity *= 0.90f; 
    } else {
        m_recoilVelocity = sf::Vector2f(0.f, 0.f);
        sf::Vector2f diff = m_homePosition - m_position;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist > 3.f) {
            sf::Vector2f dir = diff / dist;
            m_position += dir * m_speed * dt;
            if (dir.x < -0.1f) m_sprite.setScale(-1.f, 1.f);
            else if (dir.x > 0.1f) m_sprite.setScale(1.f, 1.f);
        } else {
            m_sprite.setScale(1.f, 1.f);
        }
    }
    m_sprite.setRotation(0.f); 
    m_sprite.setPosition(m_position);
    keepInsideWindow(windowSize);
}

void CWarriorBee::draw(sf::RenderWindow& window) const { CBee::draw(window); }
void CWarriorBee::resetAttackTimer() { m_attackTimer = m_attackFrequency; }
void CWarriorBee::levelUpStats(float factor) { m_speed *= (1.0f + factor); m_force += 1; }

// --- HOVER ---
std::string CWarriorBee::getStats() const {
    return "GUERRIERE\nForce: " + std::to_string(m_force) + "\nVitesse: " + std::to_string((int)m_speed);
}