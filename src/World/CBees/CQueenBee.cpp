#include "World/CBees/CQueenBee.hpp"
#include "Graphics/CTextureManager.hpp"
#include "Utils/CConstants.hpp"
#include <algorithm> // pour std::max
#include <sstream>
#include <iomanip>

CQueenBee::CQueenBee(sf::Vector2f pos)
    : CBee(pos, EWindowType::BEEHIVE, 30.f),
      m_spawnTimer(0.f),
      m_currentLevel(1)
{
    // --- RANDOMISATION SPAWN TIME ---
    // Valeur entre MIN et MAX
    float randomOffset = static_cast<float>(rand() % (int)(constants::QUEEN_MAX_SPAWN_TIME - constants::QUEEN_MIN_SPAWN_TIME));
    m_spawnDuration = constants::QUEEN_MIN_SPAWN_TIME + randomOffset;

    // ... (Sprite & Barres init inchangés) ...
    m_sprite.setTexture(CTextureManager::instance().getTexture("queen_bee.png"), true);
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    
    // ... (Init barres) ...
    m_barBack.setSize(sf::Vector2f(60.f, 6.f)); m_barBack.setFillColor(sf::Color::Black); m_barBack.setOutlineThickness(1.f); m_barBack.setOutlineColor(sf::Color::White); m_barBack.setOrigin(30.f, 3.f); 
    m_barFront.setSize(sf::Vector2f(0.f, 6.f)); m_barFront.setFillColor(sf::Color::Green); m_barFront.setOrigin(30.f, 3.f);
}

void CQueenBee::setHiveLevel(int level) {
    if (m_currentLevel != level) {
        m_currentLevel = level;
        // La durée est la base aléatoire moins 1 seconde par niveau
        m_spawnDuration = std::max(5.0f, m_baseSpawnDuration - (float)(level - 1));
    }
}
void CQueenBee::update(float dt, const sf::Vector2u& windowSize)
{
    sf::Vector2f pos = getPosition();
    setPosition(pos);
    keepInsideWindow(windowSize);

    // Logique Spawn
    if (m_spawnTimer < m_spawnDuration) {
        m_spawnTimer += dt;
        if (m_spawnTimer > m_spawnDuration) m_spawnTimer = m_spawnDuration;
    }

    // Mise à jour graphique barre
    float ratio = m_spawnTimer / m_spawnDuration;
    m_barFront.setSize(sf::Vector2f(60.f * ratio, 6.f));

    sf::Vector2f barPos = pos;
    barPos.y -= 40.f;

    m_barBack.setPosition(barPos);
    m_barFront.setPosition(barPos.x - 30.f, barPos.y - 3.f);
    m_barFront.setOrigin(0.f, 0.f);
}

void CQueenBee::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
    window.draw(m_barBack);
    window.draw(m_barFront);
}

void CQueenBee::levelUpStats(float factor) {
    (void)factor; // Pour éviter le warning
}

bool CQueenBee::isReadyToSpawn() const {
    return m_spawnTimer >= m_spawnDuration;
}

void CQueenBee::resetSpawnTimer() {
    m_spawnTimer = 0.f;
}

// Hover Info
std::string CQueenBee::getStats() const {
    std::stringstream ss;
    ss << "REINE (Niv " << m_currentLevel << ")\n";
    ss << "Spawn: " << std::fixed << std::setprecision(1) << m_spawnTimer << " / " << m_spawnDuration << "s";
    return ss.str();
}