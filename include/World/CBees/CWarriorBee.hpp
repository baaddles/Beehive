#pragma once
#include "World/CBees/CBee.hpp"
#include "Utils/CEnums.hpp"

class CWarriorBee : public CBee
{
private:
    EWarriorState m_state;
    int m_force;
    float m_attackFrequency;
    float m_attackTimer;

public:
    CWarriorBee(sf::Vector2f pos, EWindowType win);

    bool canAttack() const { return m_attackTimer <= 0.f; }
    void resetAttackTimer() { m_attackTimer = m_attackFrequency; }
    int getForce() const { return m_force; }
    void levelUpStats(float factor) override;

    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;
};