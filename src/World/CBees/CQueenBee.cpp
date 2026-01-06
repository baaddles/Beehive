#include "World/CBees/CQueenBee.hpp"
#include "Graphics/CTextureManager.hpp"

CQueenBee::CQueenBee(sf::Vector2f pos)
    : CBee(pos, EWindowType::BEEHIVE, 30.f), // Vitesse réduite pour la reine
      m_reproduction(false),
      m_reproductionFrequency(5.f),
      m_verticalDirection(1.f)
{
    // Le paramètre 'true' réinitialise le rectangle de texture à la taille de la nouvelle image
    m_sprite.setTexture(CTextureManager::instance().getTexture("queen_bee.png"), true);
    
    // Recalcul de l'origine suite au changement de texture
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void CQueenBee::update(float dt, const sf::Vector2u& windowSize)
{
    sf::Vector2f pos = getPosition();
    
    // Mouvement vertical fluide
    pos.y += m_speed * dt * m_verticalDirection;
    
    // On rebondit avant de toucher le bord (marge de 10% de la fenêtre)
    if (pos.y < windowSize.y * 0.1f || pos.y > windowSize.y * 0.9f)
    {
        m_verticalDirection *= -1.f;
    }

    setPosition(pos);
    keepInsideWindow(windowSize);
}

void CQueenBee::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}