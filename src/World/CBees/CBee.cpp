#include "World/CBees/CBee.hpp"
#include "Graphics/CTextureManager.hpp"
#include "Utils/CConstants.hpp"

CBee::CBee(sf::Vector2f pos, EWindowType win, float speed)
    : CEntity(pos, win), 
      m_speed(speed),
      m_homePosition(pos)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("default.png"), true);
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    
    // On utilise le setPosition surchargé pour initialiser correctement
    setPosition(m_position);
}

void CBee::update(float dt, const sf::Vector2u& windowSize)
{
    (void)dt; (void)windowSize;
}

void CBee::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}

sf::FloatRect CBee::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

// --- CORRECTION : MISE A JOUR IMMEDIATE ---
void CBee::setPosition(sf::Vector2f pos) {
    // 1. On met à jour la donnée logique (dans le parent)
    CEntity::setPosition(pos);
    
    // 2. On met à jour le visuel IMMÉDIATEMENT
    m_sprite.setPosition(pos);
}

void CBee::keepInsideWindow(const sf::Vector2u& windowSize)
{
    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    float halfHeight = bounds.height / 2.f;
    
    if (m_position.y < halfHeight) m_position.y = halfHeight;
    if (m_position.y > (float)windowSize.y - halfHeight) m_position.y = (float)windowSize.y - halfHeight;

    // Utilisation de la méthode surchargée
    setPosition(m_position);
}