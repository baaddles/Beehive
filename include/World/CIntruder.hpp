#pragma once
#include "World/CEntity.hpp"

class CIntruder : public CEntity
{
private:
    int m_life; // Vie de l'intrus
    int m_force; // Dégats envers les abeilles
    int m_attackFrequency; // Fréquence d'attaque

public:
    CIntruder(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};