#include "World/CIntruder.hpp"
#include "Graphics/CTextureManager.hpp"
#include <cmath>

// Constructeur mis à jour avec HP, Force et Vitesse
CIntruder::CIntruder(sf::Vector2f pos, int hp, int force, float speed)
    : CEntity(pos, EWindowType::OUTSIDE), 
      m_life(hp), 
      m_force(force), 
      m_speed(speed) // On utilise la vitesse calculée par la vague
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("intruder.png"), true);
    
    // Centrage de l'origine du sprite
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    
    m_sprite.setPosition(m_position);
}

void CIntruder::update(float dt) {
    // 1. Mouvement linéaire vers la GAUCHE
    m_position.x -= m_speed * dt;
    
    // 2. Petit mouvement sinusoïdal pour simuler le vol
    static float timer = 0;
    timer += dt;
    m_position.y += std::sin(timer * 5.f) * 0.5f;

    m_sprite.setPosition(m_position);
    
    // 3. Feedback visuel sur les dégâts
    if (m_life < 20) {
        m_sprite.setColor(sf::Color(255, 50, 50)); 
    }
}

void CIntruder::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void CIntruder::takeDamage(int amount) {
    m_life -= amount;
}

bool CIntruder::isDead() const {
    return m_life <= 0;
}

sf::FloatRect CIntruder::getBounds() const {
    return m_sprite.getGlobalBounds();
}