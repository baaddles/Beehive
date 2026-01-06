#include "World/CBees/CWorkerBee.hpp"
#include "Utils/CConstants.hpp"
#include <cstdlib>
#include <cmath>

CWorkerBee::CWorkerBee(sf::Vector2f pos, EWindowType winType)
    : CBee(pos, winType, 100.f), 
      m_pollenCollected(0), 
      m_capacity(constants::WORKER_CAPACITY)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("worker_bee.png"), true);
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void CWorkerBee::update(float dt, const sf::Vector2u& windowSize)
{
    // Errance aléatoire
    static float timer = 0;
    static sf::Vector2f dir(1.f, 0.f);
    timer += dt;
    if (timer > 0.8f) {
        float angle = (std::rand() % 360) * 3.14159f / 180.f;
        dir = sf::Vector2f(std::cos(angle), std::sin(angle));
        timer = 0;
    }

    m_position += dir * m_speed * dt;
    setPosition(m_position);
    keepInsideWindow(windowSize);
}

void CWorkerBee::draw(sf::RenderWindow& window) const
{
    // Si pleine, on peut la teinter légèrement en jaune pour le debug
    if (isFull()) {
        const_cast<sf::Sprite&>(m_sprite).setColor(sf::Color(255, 255, 150));
    } else {
        const_cast<sf::Sprite&>(m_sprite).setColor(sf::Color::White);
    }
    window.draw(m_sprite);
}

bool CWorkerBee::isFull() const {
    return m_pollenCollected >= m_capacity;
}

void CWorkerBee::addPollen(int amount) {
    m_pollenCollected += amount;
    if (m_pollenCollected > m_capacity) m_pollenCollected = m_capacity;
}

void CWorkerBee::resetPollen() {
    m_pollenCollected = 0;
}