#pragma once
#include <vector>
#include <memory>
#include "World/CBees/CBee.hpp"

class CFlower;

enum class EWorkerBehavior {
    WANDERING,
    GOING_TO_FLOWER,
    COLLECTING,
    RETURNING,
    DELIVERING // Nouvel état pour aller au centre de la ruche
};

class CWorkerBee : public CBee
{
private:
    int m_pollenCollected;
    int m_capacity;
    float m_collectionTimer;
    float m_collectionDuration;
    
    EWorkerBehavior m_behavior;
    CFlower* m_targetFlower;

public:
    CWorkerBee(sf::Vector2f pos, EWindowType winType = EWindowType::BEEHIVE);

    void update(float dt, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;

    void searchFlower(const std::vector<std::unique_ptr<CFlower>>& flowers);

    bool isFull() const;
    void resetPollen();
    void startDelivering(); // Force le passage en mode livraison
    EWorkerBehavior getBehavior() const { return m_behavior; }
};