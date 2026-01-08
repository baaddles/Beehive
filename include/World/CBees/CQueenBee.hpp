#pragma once
#include "World/CBees/CBee.hpp"
#include <SFML/Graphics.hpp>

class CQueenBee : public CBee
{
private:
    float m_spawnTimer;
    float m_spawnDuration;
    int m_currentLevel;
    sf::RectangleShape m_barBack;
    sf::RectangleShape m_barFront;

public:
    CQueenBee(sf::Vector2f pos);
    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;
    void levelUpStats(float factor) override;

    // Hover
    std::string getStats() const override;

    void setHiveLevel(int level);
    bool isReadyToSpawn() const;
    void resetSpawnTimer();
};