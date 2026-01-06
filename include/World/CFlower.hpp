#pragma once
#include "World/CEntity.hpp"

class CFlower : public CEntity
{
private:
    sf::Sprite m_sprite;
    bool m_isAvailable;
    float m_regenTimer;
    int m_pollenValue;

public:
    CFlower(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    bool isAvailable() const;
    int collect(); // Retourne le pollen et lance la régénération
    sf::FloatRect getBounds() const;
};