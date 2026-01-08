#include "World/CBees/CQueenBee.hpp"
#include "Graphics/CTextureManager.hpp"
#include <algorithm> // pour std::max

CQueenBee::CQueenBee(sf::Vector2f pos)
    : CBee(pos, EWindowType::BEEHIVE, 30.f),
      m_spawnTimer(0.f),
      m_spawnDuration(45.0f), // 45 secondes au début
      m_currentLevel(1)
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("queen_bee.png"), true);
    
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Initialisation de la barre de progression
    m_barBack.setSize(sf::Vector2f(60.f, 6.f));
    m_barBack.setFillColor(sf::Color::Black);
    m_barBack.setOutlineThickness(1.f);
    m_barBack.setOutlineColor(sf::Color::White);
    m_barBack.setOrigin(30.f, 3.f); 

    m_barFront.setSize(sf::Vector2f(0.f, 6.f));
    m_barFront.setFillColor(sf::Color::Green);
    m_barFront.setOrigin(30.f, 3.f);
}

void CQueenBee::setHiveLevel(int level) {
    m_currentLevel = level;
    // Formule : 45 secondes - 1 seconde par niveau
    // Minimum 5 secondes pour garder un délai
    m_spawnDuration = std::max(5.0f, 45.0f - (float)(level - 1));
}

void CQueenBee::update(float dt, const sf::Vector2u& windowSize)
{
    // Mouvement standard
    sf::Vector2f pos = getPosition();
    setPosition(pos);
    keepInsideWindow(windowSize);

    // --- LOGIQUE SPAWN ---
    if (m_spawnTimer < m_spawnDuration) {
        m_spawnTimer += dt;
        if (m_spawnTimer > m_spawnDuration) m_spawnTimer = m_spawnDuration;
    }

    // Mise à jour visuelle de la barre
    float ratio = m_spawnTimer / m_spawnDuration;
    // La barre grandit de 0 à 60 pixels
    m_barFront.setSize(sf::Vector2f(60.f * ratio, 6.f));

    // Positionnement de la barre au-dessus de la reine
    sf::Vector2f barPos = pos;
    barPos.y -= 40.f; // Décalage vers le haut

    m_barBack.setPosition(barPos);
    
    // Astuce pour l'alignement gauche malgré l'origine centrée du Back :
    // On cale le Front par rapport au coin gauche du Back
    m_barFront.setPosition(barPos.x - 30.f, barPos.y - 3.f);
    m_barFront.setOrigin(0.f, 0.f);
}

void CQueenBee::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
    // Dessin de l'UI locale après le sprite
    window.draw(m_barBack);
    window.draw(m_barFront);
}

void CQueenBee::levelUpStats(float factor) {
    // La reine n'a pas de stats de combat pour l'instant
}

bool CQueenBee::isReadyToSpawn() const {
    return m_spawnTimer >= m_spawnDuration;
}

void CQueenBee::resetSpawnTimer() {
    m_spawnTimer = 0.f;
}