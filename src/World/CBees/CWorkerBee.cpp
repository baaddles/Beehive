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
      m_targetFlower(nullptr)
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
    
    // Si on est ici, c'est qu'aucune fleur n'est disponible
    m_behavior = EWorkerBehavior::RETURNING;
}

void CWorkerBee::startDelivering() {
    m_behavior = EWorkerBehavior::DELIVERING;
}

void CWorkerBee::update(float dt, const sf::Vector2u& windowSize)
{
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

    if (m_behavior == EWorkerBehavior::GOING_TO_FLOWER && m_targetFlower) {
        targetPos = m_targetFlower->getPosition();
        hasTarget = true;
    } 
    else if (m_behavior == EWorkerBehavior::RETURNING) {
        targetPos = sf::Vector2f(-100.f, m_position.y);
        hasTarget = true;
    }
    else if (m_behavior == EWorkerBehavior::DELIVERING) {
        targetPos = sf::Vector2f(constants::HIVE_CENTER_X, constants::HIVE_CENTER_Y);
        hasTarget = true;
    }

    if (hasTarget) {
        sf::Vector2f diff = targetPos - m_position;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist > 5.f) {
            m_position += (diff / dist) * m_speed * dt;
        } else {
            if (m_behavior == EWorkerBehavior::GOING_TO_FLOWER) {
                m_behavior = EWorkerBehavior::COLLECTING;
                m_collectionTimer = m_collectionDuration;
            } else if (m_behavior == EWorkerBehavior::DELIVERING) {
                resetPollen();
            }
        }
    } else {
        // Mode WANDERING (Errance plus lente)
        static float wanderTimer = 0;
        static sf::Vector2f wanderDir(1.f, 0.f);
        wanderTimer += dt;
        if (wanderTimer > 1.2f) {
            float angle = (std::rand() % 360) * 3.14159f / 180.f;
            wanderDir = sf::Vector2f(std::cos(angle), std::sin(angle));
            wanderTimer = 0;
        }
        m_position += wanderDir * m_speed * dt;
    }

    setPosition(m_position);
    keepInsideWindow(windowSize);
}

void CWorkerBee::draw(sf::RenderWindow& window) const {
    if (isFull()) const_cast<sf::Sprite&>(m_sprite).setColor(sf::Color::Yellow);
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
    // Augmente la vitesse de 10%
    m_speed *= (1.0f + factor); 
    
    // Augmente la capacité de pollen de 1 tous les 2 niveaux (par exemple)
    m_capacity += 1; 
    
    std::cout << "Worker a gagne en competence ! Vitesse: " << m_speed << ", Capacite: " << m_capacity << std::endl;
}