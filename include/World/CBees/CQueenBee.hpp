#pragma once
#include "World/CBees/CBee.hpp"

class CQueenBee : public CBee
{
private:
    bool m_reproduction;              // Si la reine crée une abeille
    float m_reproductionFrequency;    // Temps entre créations
    float m_verticalDirection;        // Mouvement haut/bas

public:
    CQueenBee(sf::Vector2f pos);

    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;
};
