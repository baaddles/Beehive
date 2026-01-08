#pragma once
#include "World/CEntity.hpp"

class CIntruder : public CEntity
{
private:
    sf::Sprite m_sprite;
    int m_life; // Vie de l'intrus
    int m_force; // Dégats envers les abeilles
    int m_attackFrequency; // Fréquence d'attaque

public:
    CIntruder(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    void takeDamage(int amount);
    bool isDead() const;
    int getLife() const { return m_life; }

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const { return m_position; }
};