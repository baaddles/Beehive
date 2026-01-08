#include "World/CBees/CWorkerBee.hpp"
#include "World/CFlower.hpp"
#include "Utils/CConstants.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>

CWorkerBee::CWorkerBee(sf::Vector2f pos, EWindowType winType)
    : CBee(pos, winType, constants::WORKER_SPEED), 
      m_pollenCollected(0), 
      m_capacity(constants::WORKER_CAPACITY),
      m_collectionTimer(0.f),
      m_collectionDuration(2.0f),
      m_behavior(EWorkerBehavior::WANDERING),
      m_targetFlower(nullptr),
      m_wanderSeed((float)(rand() % 100))
{
    m_sprite.setTexture(CTextureManager::instance().getTexture("worker_bee.png"), true);
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void CWorkerBee::searchFlower(const std::vector<std::unique_ptr<CFlower>>& flowers) {
    if (m_behavior != EWorkerBehavior::WANDERING || isFull()) return;

    for (auto& flower : flowers) {
        if (flower->isAvailable()) {
            m_targetFlower = flower.get();
            m_targetFlower->setReserved(true);
            m_behavior = EWorkerBehavior::GOING_TO_FLOWER;
            return;
        }
    }
    m_behavior = EWorkerBehavior::RETURNING;
}

void CWorkerBee::startDelivering() {
    m_behavior = EWorkerBehavior::DELIVERING;
}

void CWorkerBee::update(float dt, const sf::Vector2u& windowSize)
{
    // --- GESTION ETAT RECOLTE ---
    if (m_behavior == EWorkerBehavior::COLLECTING) {
        m_collectionTimer -= dt;
        if (m_collectionTimer <= 0) {
            if (m_targetFlower) {
                m_pollenCollected += m_targetFlower->collect();
            }
            m_targetFlower = nullptr;
            m_behavior = isFull() ? EWorkerBehavior::RETURNING : EWorkerBehavior::WANDERING;
        }
        return; 
    }

    sf::Vector2f targetPos = m_position;
    bool hasTarget = false;

    // --- DEFINITION CIBLE ---
    if (m_behavior == EWorkerBehavior::GOING_TO_FLOWER && m_targetFlower) {
        targetPos = m_targetFlower->getPosition();
        hasTarget = true;
    } 
    else if (m_behavior == EWorkerBehavior::RETURNING) {
        targetPos = sf::Vector2f(-100.f, m_position.y); // Gauche
        hasTarget = true;
    }
    else if (m_behavior == EWorkerBehavior::DELIVERING) {
        targetPos = sf::Vector2f(constants::HIVE_CENTER_X, constants::HIVE_CENTER_Y);
        hasTarget = true;
    }

    // --- MOUVEMENT ---
    sf::Vector2f movementDir(0.f, 0.f);

    if (hasTarget) {
        sf::Vector2f diff = targetPos - m_position;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        
        if (dist > 5.f) {
            movementDir = (diff / dist);
            m_position += movementDir * m_speed * dt;
        } else {
            // Arrivée à destination
            if (m_behavior == EWorkerBehavior::GOING_TO_FLOWER) {
                m_behavior = EWorkerBehavior::COLLECTING;
                m_collectionTimer = m_collectionDuration;
            } else if (m_behavior == EWorkerBehavior::DELIVERING) {
                resetPollen();
            }
        }
    } 
    else {
        // --- ANIMATION WANDER (Sinusoidale Haut/Bas) ---
        static float globalTime = 0.f;
        globalTime += dt;
        
        // Mouvement Vertical (Oscillation)
        // m_wanderSeed permet de désynchroniser les abeilles
        float waveY = std::sin(globalTime * 3.f + m_wanderSeed) * 0.5f; 
        
        // Mouvement Horizontal (Lent drift)
        // On utilise cos pour le X pour faire des cercles/ovales aplatis
        float waveX = std::cos(globalTime * 1.f + m_wanderSeed) * 0.3f;

        m_position.x += waveX * m_speed * 0.5f * dt;
        m_position.y += waveY * m_speed * 0.5f * dt;

        movementDir = sf::Vector2f(waveX, waveY);
    }

    // --- ORIENTATION VISUELLE (FLIP) ---
    // On ne touche JAMAIS à la rotation (setRotation(0)).
    // On flip le scale X.
    if (movementDir.x < -0.1f) {
        m_sprite.setScale(-1.f, 1.f); // Regarde à gauche
    } else if (movementDir.x > 0.1f) {
        m_sprite.setScale(1.f, 1.f);  // Regarde à droite
    }
    m_sprite.setRotation(0.f);

    setPosition(m_position);
    keepInsideWindow(windowSize);
}

void CWorkerBee::draw(sf::RenderWindow& window) const {
    if (m_pollenCollected > 0) const_cast<sf::Sprite&>(m_sprite).setColor(sf::Color::Yellow);
    else const_cast<sf::Sprite&>(m_sprite).setColor(sf::Color::White);
    window.draw(m_sprite);
}

bool CWorkerBee::isFull() const { return m_pollenCollected >= m_capacity; }

void CWorkerBee::resetPollen() { 
    m_pollenCollected = 0; 
    m_behavior = EWorkerBehavior::WANDERING; 
    if(m_targetFlower) m_targetFlower->setReserved(false);
    m_targetFlower = nullptr;
}

void CWorkerBee::levelUpStats(float factor) {
    m_speed *= (1.0f + factor); 
    m_capacity += 1; 
}