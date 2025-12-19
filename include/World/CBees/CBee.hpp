#pragma once
#include "World/CEntity.hpp"

class CBee : public CEntity
{
protected:
    int m_life; // Vie de l'abeille, commun à chaque abeille (mais différentes utilisations)

public:
    CBee(sf::Vector2f pos, EWindowType win, int life);
    virtual bool isAlive() const;
};