#include "World/CFlower.hpp"
#include "Graphics/CTextureManager.hpp"

CFlower::CFlower(sf::Vector2f pos)
    : CEntity(pos, EWindowType::OUTSIDE), m_pollen(100), m_usable(true)
{
}

void CFlower::update(float dt)
{
    // Logique de régénération de pollen
}

void CFlower::draw(sf::RenderWindow& window) const
{
    // Pour l'instant on ne dessine rien ou un sprite par défaut
}