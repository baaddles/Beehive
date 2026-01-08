#pragma once
#include "World/CEntity.hpp"
#include <string>

class CIntruder : public CEntity
{
private:
    sf::Sprite m_sprite;
    int m_life;
    int m_force;
    float m_speed;

public:
    CIntruder(sf::Vector2f pos, int hp, int force, float speed);

    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;

    // --- CORRECTION : Ajout de la déclaration manquante ---
    std::string getStats() const override;
    sf::FloatRect getBounds() const override;

    void takeDamage(int amount);
    bool isDead() const;
    int getForce() const { return m_force; }
    int getLife() const { return m_life; }
    sf::Vector2f getPosition() const { return m_position; }
};