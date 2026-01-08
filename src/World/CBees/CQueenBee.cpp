#include "World/CBees/CQueenBee.hpp"
#include "Graphics/CTextureManager.hpp"

CQueenBee::CQueenBee(sf::Vector2f pos)
    : CBee(pos, EWindowType::BEEHIVE, 30.f), // Vitesse réduite pour la reine
      m_reproduction(false),
      m_reproductionFrequency(5.f)
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
    setPosition(pos);
    keepInsideWindow(windowSize);
}

void CQueenBee::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}

void CQueenBee::levelUpStats(float factor) {
    // La reine pourrait augmenter sa fréquence de reproduction plus tard
    // Pour l'instant, on ne fait rien
}