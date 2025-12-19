#pragma once
#include "World/CEntity.hpp"

class CFlower : public CEntity
{
private:
    int m_pollen; // Quantité de pollen dans la fleur pouvant être récoltée par une abeille ouvrière
    bool m_usable; // Si elle est utilisable, m_usable = TRUE. Si elle a été utilisé par une abeille, alors m_usable = FALSE, et se régénère au bout d'une minute.

public:
    CFlower(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};