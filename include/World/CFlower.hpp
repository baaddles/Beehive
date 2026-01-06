#pragma once
#include "World/CEntity.hpp"

class CFlower : public CEntity
{
private:
    int m_pollen;
    bool m_usable;

public:
    CFlower(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};