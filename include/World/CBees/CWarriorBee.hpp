#pragma once
#include "World/CBees/CBee.hpp"

class CWarriorBee : public CBee
{
private:
    EWarriorState m_state;
    int m_force;
    float m_attackFrequency;
    float m_attackTimer;

    sf::Vector2f m_patrolPosition; // Position de garde
    
    // NOUVEAU : Pour le recul fluide
    sf::Vector2f m_recoilVelocity; 

public:
    CWarriorBee(sf::Vector2f pos, EWindowType win);

    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;
    void levelUpStats(float factor) override;

    // Combat
    bool canAttack() const { return m_attackTimer <= 0.f; }
    void resetAttackTimer();
    int getForce() const { return m_force; }
    
    // NOUVEAU : Applique une impulsion
    void applyKnockback(sf::Vector2f direction, float force);

    // Navigation
    void setPatrolPosition(const sf::Vector2f& pos) { m_patrolPosition = pos; }
    sf::Vector2f getPatrolPosition() const { return m_patrolPosition; }
};