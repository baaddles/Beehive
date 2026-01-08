#include "World/CIntruder.hpp"
#include "Graphics/CTextureManager.hpp"
#include <cmath>

// Constructeur mis à jour avec HP et Force
CIntruder::CIntruder(sf::Vector2f pos, int hp, int force)
    : CEntity(pos, EWindowType::OUTSIDE), 
      m_life(hp), 
      m_force(force), 
      m_speed(23.33f) // Calibrage : 700 pixels / 30 secondes = 23.33 px/s
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("intruder.png"), true);
    
    // Centrage de l'origine du sprite pour des rotations ou placements propres
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    
    m_sprite.setPosition(m_position);

    // Optionnel : Si l'intrus est très fort (vagues avancées), on le teinte légèrement
    if (m_force > 15) {
        m_sprite.setColor(sf::Color(255, 200, 200)); 
    }
}

void CIntruder::update(float dt) {
    // 1. Mouvement linéaire vers la GAUCHE (vers la ruche qui est à x=0 dans Outside)
    m_position.x -= m_speed * dt;
    
    // 2. Petit mouvement sinusoïdal pour simuler le vol
    // On utilise une variable statique pour le temps (partagée par tous) 
    // ou on pourrait ajouter m_flightTime dans le .hpp pour qu'ils soient désynchronisés.
    static float timer = 0;
    timer += dt;
    
    // On ajoute une petite variation sur Y
    m_position.y += std::sin(timer * 5.f) * 0.5f;

    // 3. Application de la position
    m_sprite.setPosition(m_position);
    
    // 4. Feedback visuel sur les dégâts (Logique déplacée ici, pas dans draw)
    // Si la vie est critique (moins de 20 PV), il clignote ou devient rouge
    if (m_life < 20) {
        m_sprite.setColor(sf::Color(255, 50, 50)); 
    }
}

// Draw est const, on ne modifie rien ici, on affiche juste
void CIntruder::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void CIntruder::takeDamage(int amount) {
    m_life -= amount;
}

bool CIntruder::isDead() const {
    return m_life <= 0;
}

sf::FloatRect CIntruder::getBounds() const {
    return m_sprite.getGlobalBounds();
}