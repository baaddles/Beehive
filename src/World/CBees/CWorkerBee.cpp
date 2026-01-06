#include "World/CBees/CWorkerBee.hpp"
#include <cstdlib>
#include <cmath>

CWorkerBee::CWorkerBee(sf::Vector2f pos, EWindowType winType)
    : CBee(pos, winType, 80.f), m_hasPollen(false)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("worker_bee.png"));
}

void CWorkerBee::update(float dt, const sf::Vector2u& windowSize)
{
    sf::Vector2f pos = getPosition();
    
    // Logique de mouvement : "Wander" (errance aléatoire)
    static float timer = 0;
    static sf::Vector2f dir(1.f, 0.f);
    timer += dt;
    
    if (timer > 0.5f) { // Change de direction toutes les 0.5s
        float angle = (std::rand() % 360) * 3.14159f / 180.f;
        dir = sf::Vector2f(std::cos(angle), std::sin(angle));
        timer = 0;
    }

    pos += dir * m_speed * dt;
    setPosition(pos);

    // Gestion des bords et transition
    // Si l'abeille touche le bord GAUCHE (sortie de ruche) ou DROITE (entrée de ruche)
    if (m_windowType == EWindowType::BEEHIVE && !m_hasPollen) {
        if (pos.x <= 0) {
            // L'application s'occupera du switch, on se contente de marquer le souhait
            m_position.x = windowSize.x - 10; // Prête à apparaître à droite de l'autre fenêtre
        }
    } else if (m_windowType == EWindowType::OUTSIDE && m_hasPollen) {
        if (pos.x >= windowSize.x - m_sprite.getGlobalBounds().width) {
            m_position.x = 10; // Prête à apparaître à gauche de l'autre fenêtre
        }
    }

    keepInsideWindow(windowSize);
}

void CWorkerBee::draw(sf::RenderWindow& window) const
{
    // On pourrait changer la couleur si elle a du pollen pour aider au debug
    if (m_hasPollen) {
        const_cast<sf::Sprite&>(m_sprite).setColor(sf::Color::Yellow);
    } else {
        const_cast<sf::Sprite&>(m_sprite).setColor(sf::Color::White);
    }
    CBee::draw(window);
}