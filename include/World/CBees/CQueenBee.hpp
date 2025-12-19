#pragma once
#include "World/CBees/CBee.hpp"

class CQueenBee : public CBee
{
private:
    bool m_reproduction; // Permet de savoir si la reine est en train de créer une abeille. Si m_reproduction = TRUE alors elle crée une abeille
    float m_reproductionFrequency; // Vitesse de création d'une abeille en secondes

public:
    CQueenBee(sf::Vector2f pos); // pos : Coordonnées sur la fenêtre

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};