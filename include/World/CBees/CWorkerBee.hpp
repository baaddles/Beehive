#pragma once
#include "World/CBees/CBee.hpp"
#include "World/CFlower.hpp"
#include <vector>
#include <memory>

enum class EWorkerBehavior { WANDERING, GOING_TO_FLOWER, COLLECTING, RETURNING, DELIVERING };

class CWorkerBee : public CBee
{
private:
    int m_pollenCollected;
    int m_capacity;
    float m_collectionTimer;
    float m_collectionDuration;
    float m_wanderSeed;
    EWorkerBehavior m_behavior;
    CFlower* m_targetFlower;

public:
    CWorkerBee(sf::Vector2f pos, EWindowType winType);
    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;
    void levelUpStats(float factor) override;

    // Hover
    std::string getStats() const override;

    void searchFlower(const std::vector<std::unique_ptr<CFlower>>& flowers);
    void startDelivering();
    bool isFull() const;
    void resetPollen();
    int getPollenCollected() const { return m_pollenCollected; }
    EWorkerBehavior getBehavior() const { return m_behavior; }
};