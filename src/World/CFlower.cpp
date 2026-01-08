#include "World/CFlower.hpp"
#include "Graphics/CTextureManager.hpp"
#include "Utils/CConstants.hpp"
#include <string>

CFlower::CFlower(sf::Vector2f pos)
    : CEntity(pos, EWindowType::OUTSIDE), 
      m_isAvailable(true), 
      m_isReserved(false),
      m_regenTimer(0.f),
      m_pollenValue(constants::POLLEN_PER_FLOWER)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("flower.png"), true);
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    m_sprite.setPosition(m_position);
}

void CFlower::update(float dt, const sf::Vector2u& windowSize) {
    (void)windowSize; // CORRECTION : Silence warning

    if (!m_isAvailable) {
        m_regenTimer -= dt;
        if (m_regenTimer <= 0.f) {
            m_isAvailable = true;
            m_isReserved = false;
        }
    }
}

void CFlower::draw(sf::RenderWindow& window) const {
    if (m_isAvailable) {
        window.draw(m_sprite);
    }
}

bool CFlower::isAvailable() const { 
    return m_isAvailable && !m_isReserved; 
}

int CFlower::collect() {
    m_isAvailable = false;
    m_isReserved = false;
    m_regenTimer = constants::FLOWER_REGEN_TIME;
    return m_pollenValue;
}

sf::FloatRect CFlower::getBounds() const { 
    return m_sprite.getGlobalBounds(); 
}

std::string CFlower::getStats() const {
    return "FLEUR\nPollen: " + std::to_string(m_pollenValue);
}