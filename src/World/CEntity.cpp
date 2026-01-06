#include "World/CEntity.hpp"

CEntity::CEntity(sf::Vector2f pos, EWindowType windowType)
    : m_position(pos), m_windowType(windowType)
{
}

EWindowType CEntity::getWindowType() const
{
    return m_windowType;
}
