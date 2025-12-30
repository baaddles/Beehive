#include "World/CBees/CBee.hpp"

CBee::CBee(sf::Vector2f pos, EWindowType win, int life)
    : CEntity(pos, win),
      m_life(life)
{
}

bool CBee::isAlive() const
{
    return m_life > 0;
}
