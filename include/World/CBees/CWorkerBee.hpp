#pragma once
#include "CBee.hpp"

class CWorkerBee : public CBee
{
private:
    bool m_hasPollen;

public:
    CWorkerBee(sf::Vector2f pos, EWindowType winType = EWindowType::BEEHIVE);

    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;

    bool hasPollen() const { return m_hasPollen; }
    void setHasPollen(bool val) { m_hasPollen = val; }
};