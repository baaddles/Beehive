#include "World/CEntity.hpp"

// Implémentation du Constructeur
CEntity::CEntity(sf::Vector2f pos, EWindowType windowType) 
    : m_position(pos), m_windowType(windowType) 
{
}

// Implémentation par défaut de getStats
std::string CEntity::getStats() const {
    return ""; 
}

// Implémentation des Getters / Setters
EWindowType CEntity::getWindowType() const { 
    return m_windowType; 
}

void CEntity::setWindowType(EWindowType type) { 
    m_windowType = type; 
}

sf::Vector2f CEntity::getPosition() const { 
    return m_position; 
}

void CEntity::setPosition(sf::Vector2f pos) { 
    m_position = pos; 
}