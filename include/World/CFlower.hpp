#pragma once
#include "World/CEntity.hpp"

class CFlower : public CEntity
{
private:
    sf::Sprite m_sprite;
    bool m_isAvailable;
    bool m_isReserved; // Pour l'exclusivité
    float m_regenTimer;
    int m_pollenValue;

public:
    CFlower(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    bool isAvailable() const;
    void setReserved(bool reserved) { m_isReserved = reserved; }
    
    int collect();
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
};