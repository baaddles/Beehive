#include "World/CBees/CBee.hpp"
#include "Graphics/CTextureManager.hpp"
#include "Utils/CConstants.hpp"

CBee::CBee(sf::Vector2f pos, EWindowType win, float speed)
    : CEntity(pos, win), // Appel du constructeur de CEntity
      m_speed(speed),
      m_homePosition(pos)
{
    // Texture par défaut (sera écrasée par les classes filles)
    m_sprite.setTexture(CTextureManager::instance().getTexture("default.png"), true);
    
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    m_sprite.setPosition(m_position);
}

void CBee::update(float dt, const sf::Vector2u& windowSize)
{
    // Implémentation vide par défaut, surchargée par les filles
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

void CBee::keepInsideWindow(const sf::Vector2u& windowSize)
{
    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    float halfHeight = bounds.height / 2.f;
    
    // On contraint seulement Y (Haut/Bas)
    if (m_position.y < halfHeight) m_position.y = halfHeight;
    if (m_position.y > (float)windowSize.y - halfHeight) m_position.y = (float)windowSize.y - halfHeight;

    m_sprite.setPosition(m_position);
}