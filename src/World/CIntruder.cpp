#include "World/CIntruder.hpp"
#include "Graphics/CTextureManager.hpp"
#include <cmath>

CIntruder::CIntruder(sf::Vector2f pos)
    : CEntity(pos, EWindowType::OUTSIDE), m_life(100), m_force(5), m_attackFrequency(1.0f)
{
    // On utilise une texture de frelon ou une texture par défaut
    m_sprite.setTexture(CTextureManager::instance().getTexture("intruder.png"), true);
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    m_sprite.setPosition(m_position);
}

void CIntruder::update(float dt) {
    static float timer = 0;
    timer += dt;
    
    m_position.y += std::sin(timer * 2.f) * 0.5f; 

    if (m_life < 50) {
        m_sprite.setColor(sf::Color(255, 100, 100)); 
    }
    
    m_sprite.setPosition(m_position);
}

void CIntruder::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

sf::FloatRect CIntruder::getBounds() const {
    return m_sprite.getGlobalBounds();
}

void CIntruder::takeDamage(int amount) {
    m_life -= amount;
}

bool CIntruder::isDead() const {
    return m_life <= 0;
}