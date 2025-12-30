#include "World/CEntity.hpp"

CEntity::CEntity(sf::Vector2f pos, EWindowType win)
    : m_position(pos),
      m_windowType(win)
{
}
