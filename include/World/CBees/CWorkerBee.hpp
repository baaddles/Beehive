#pragma once
#include "World/CBees/CBee.hpp"
#include "Utils/CEnums.hpp"

class CWorkerBee : public CBee
{
private:
    EWorkerState m_state;
    int m_energy; // Quantité de pollen récupérable avant de retourner à la ruche
    float m_foragingSpeed; // Vitesse de pollinisation d'une fleur en secondes
    float m_movementSpeed; // Vitesse de déplacement générale (entre fleurs, retour à la ruche, contribue au temps de repos)

public:
    CWorkerBee(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};