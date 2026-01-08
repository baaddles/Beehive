#pragma once
#include "World/CEntity.hpp"

class CFlower : public CEntity {
    // ... tes attributs existants ...
    bool m_isAvailable;
    bool m_isReserved;
    float m_regenTimer;
    int m_pollenValue;
    sf::Sprite m_sprite;

public:
    CFlower(sf::Vector2f pos);
    void update(float dt, const sf::Vector2u& windowSize) override; // ou juste update(float dt) si tu adaptes CEntity
    void draw(sf::RenderWindow& window) const override;
    
    // Hover
    sf::FloatRect getBounds() const override;
    std::string getStats() const override;

    // ... autres méthodes ...
    bool isAvailable() const;
    void setReserved(bool r) { m_isReserved = r; }
    int collect();
};