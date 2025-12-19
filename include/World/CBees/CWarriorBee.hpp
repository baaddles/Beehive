#pragma once
#include "World/CBees/CBee.hpp"
#include "Utils/CEnums.hpp"

class CWarriorBee : public CBee
{
private:
    EWarriorState m_state; // état de l'abeille
    int m_force; // Dégats envers les intrus par l'abeille
    float m_attackFrequency; // Fréquence d'attaque en secondes

public:
    CWarriorBee(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};