#pragma once
#include "World/CBees/CBee.hpp"
#include "Utils/CEnums.hpp"

class CWarriorBee : public CBee
{
private:
    EWarriorState m_state;
    int m_force;
    float m_attackFrequency;

public:
    CWarriorBee(sf::Vector2f pos);

    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;
};