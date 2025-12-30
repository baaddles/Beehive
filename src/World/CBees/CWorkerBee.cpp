#include "World/CBees/CWorkerBee.hpp"

CWorkerBee::CWorkerBee(sf::Vector2f pos)
    : CBee(pos, EWindowType::OUTSIDE, 6),
      m_state(EWorkerState::DANS_RUCHE),
      m_energy(10),
      m_foragingSpeed(2.f),
      m_movementSpeed(1.f)
{
}

void CWorkerBee::update(float)
{
}

void CWorkerBee::draw(sf::RenderWindow& window)
{
    sf::CircleShape shape;
    shape.setRadius(8.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(m_position);

    window.draw(shape);
}
