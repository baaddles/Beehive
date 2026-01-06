#pragma once
#include "CBee.hpp"

class CWorkerBee : public CBee
{
private:
    int m_pollenCollected;
    int m_capacity;

public:
    CWorkerBee(sf::Vector2f pos, EWindowType winType = EWindowType::BEEHIVE);

    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;

    bool isFull() const;
    void addPollen(int amount);
    void resetPollen();
};