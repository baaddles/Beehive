#include "Core/CApplication.hpp"
#include "Utils/CConstants.hpp"
#include <iostream>
#include <cmath>

CApplication::CApplication()
    : m_hiveLevel(1),
      m_currentPollen(0),
      m_pollenForNextLevel(50),
      m_outsideWindow(sf::VideoMode(700, 500), "Outside - Nature"),
      m_insideWindow(sf::VideoMode(700, 500), "Inside - Beehive"),
      m_grassRect(sf::Vector2f(700.f, 500.f * constants::GRASS_HEIGHT_RATIO))
{
    m_outsideWindow.setFramerateLimit(60);
    m_insideWindow.setFramerateLimit(60);

    m_insideWindow.setPosition(sf::Vector2i(100, 100));
    m_outsideWindow.setPosition(sf::Vector2i(820, 100));

    m_outsideBackground.setTexture(CTextureManager::instance().getTexture("outside_bg.png"));
    m_insideBackground.setTexture(CTextureManager::instance().getTexture("hive_bg.png"));

    float grassHeight = m_grassRect.getSize().y;
    m_grassRect.setFillColor(constants::GRASS_COLOR);
    m_grassRect.setPosition(0.f, 500.f - grassHeight);

    // Initialisation des Fleurs
    for(int i = 0; i < constants::INITIAL_FLOWER_COUNT; ++i) {
        float x = 50.f + (std::rand() % 600);
        float y = (500.f - grassHeight) + (std::rand() % (int)(grassHeight - 40));
        m_flowers.push_back(std::make_unique<CFlower>(sf::Vector2f(x, y)));
    }

    // Création de la Reine
    m_entities.push_back(std::make_unique<CQueenBee>(sf::Vector2f(75.f, 250.f)));

    // Création des Ouvrières initiales
    for(int i = 0; i < 6; ++i) {
        m_entities.push_back(std::make_unique<CWorkerBee>(
            sf::Vector2f(100.f + (rand() % 400), 100.f + (rand() % 300)),
            EWindowType::BEEHIVE
        ));
    }

    // Création des Guerrières initiales (Correction du bug de variable indéfinie)
    for(int i = 0; i < 3; ++i) {
        auto warrior = std::make_unique<CWarriorBee>(sf::Vector2f(350.f, 200.f), EWindowType::BEEHIVE);
        
        // Configuration de sa position de repos
        warrior->setHomePosition(sf::Vector2f(450.f, 100.f + i * 80.f));
        warrior->setPosition(warrior->getHomePosition());
        
        // Ajout à la liste
        m_entities.push_back(std::move(warrior));
    }

    m_intruders.push_back(std::make_unique<CIntruder>(sf::Vector2f(600.f, 250.f)));
}

void CApplication::run() {
    sf::Clock clock;
    while (m_outsideWindow.isOpen() || m_insideWindow.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}

void CApplication::handleEvents() {
    sf::Event event;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        m_outsideWindow.close();
        m_insideWindow.close();
    }
    while (m_outsideWindow.pollEvent(event)) if (event.type == sf::Event::Closed) m_outsideWindow.close();
    while (m_insideWindow.pollEvent(event)) if (event.type == sf::Event::Closed) m_insideWindow.close();
}

void CApplication::update(float dt) {
    int availableFlowers = 0;
    for (auto& flower : m_flowers) {
        flower->update(dt);
        if (flower->isAvailable()) availableFlowers++;
    }

    CIntruder* targetEnnemy = nullptr;
    if (!m_intruders.empty()) {
        targetEnnemy = m_intruders[0].get();
    }

    for (auto& entity : m_entities) {
        EWindowType currentWin = entity->getWindowType();
        sf::Vector2u winSize = (currentWin == EWindowType::BEEHIVE) ? m_insideWindow.getSize() : m_outsideWindow.getSize();
        
        entity->update(dt, winSize);

        float currentSpeed = 50.f;
        CBee* bee = dynamic_cast<CBee*>(entity.get());
        if (bee) currentSpeed = bee->getSpeed();

        // LOGIQUE OUVRIÈRE
        CWorkerBee* worker = dynamic_cast<CWorkerBee*>(entity.get());
        if (worker) {
            if (currentWin == EWindowType::OUTSIDE && !worker->isFull() && worker->getBehavior() == EWorkerBehavior::WANDERING) {
                worker->searchFlower(m_flowers);
            }

            if (currentWin == EWindowType::BEEHIVE && !worker->isFull() && worker->getBehavior() == EWorkerBehavior::WANDERING && availableFlowers > 0) {
                sf::Vector2f pos = worker->getPosition();
                worker->setPosition(sf::Vector2f(pos.x + currentSpeed * dt, pos.y));
                if (pos.x >= (float)winSize.x - 5.f) {
                    worker->setWindowType(EWindowType::OUTSIDE);
                    worker->setPosition(sf::Vector2f(10.f, pos.y));
                }
            }
            else if (currentWin == EWindowType::OUTSIDE && (worker->isFull() || worker->getBehavior() == EWorkerBehavior::RETURNING)) {
                sf::Vector2f pos = worker->getPosition();
                if (pos.x <= 5.f) {
                    worker->setWindowType(EWindowType::BEEHIVE);
                    worker->setPosition(sf::Vector2f((float)winSize.x - 15.f, pos.y));
                    worker->startDelivering();
                }
            }
            else if (currentWin == EWindowType::BEEHIVE && worker->getBehavior() == EWorkerBehavior::DELIVERING) {
                sf::Vector2f diff = sf::Vector2f(constants::HIVE_CENTER_X, constants::HIVE_CENTER_Y) - worker->getPosition();
                float distSq = diff.x * diff.x + diff.y * diff.y;
                if (distSq < 400.f) { // Distance de 20 pixels
                    addPollenToHive(worker->getPollenCollected());
                    worker->resetPollen();
                }
            }
        }

        // LOGIQUE GUERRIÈRE
        CWarriorBee* warrior = dynamic_cast<CWarriorBee*>(entity.get());
        if (warrior) {
            if (targetEnnemy) {
                warrior->setHomePosition(targetEnnemy->getPosition());
                if (currentWin == EWindowType::OUTSIDE && !targetEnnemy->isDead()) {
                    if (warrior->canAttack() && warrior->getBounds().intersects(targetEnnemy->getBounds())) {
                        targetEnnemy->takeDamage(warrior->getForce());
                        warrior->resetAttackTimer();
                        std::cout << "Attaque ! Vie restant : " << targetEnnemy->getLife() << std::endl;
                    }
                }
                else if (currentWin == EWindowType::BEEHIVE) {
                    sf::Vector2f pos = warrior->getPosition();
                    warrior->setPosition(sf::Vector2f(pos.x + currentSpeed * dt, pos.y));
                    if (pos.x >= (float)winSize.x - 5.f) {
                        warrior->setWindowType(EWindowType::OUTSIDE);
                        warrior->setPosition(sf::Vector2f(10.f, pos.y));
                    }
                }
            }
        }
    }

    // Mise à jour et nettoyage des intrus
    for (auto it = m_intruders.begin(); it != m_intruders.end(); ) {
        (*it)->update(dt);
        if ((*it)->isDead()) {
            it = m_intruders.erase(it);
            std::cout << "L'intrus a ete vaincu !" << std::endl;
        } else {
            ++it;
        }
    }
}

void CApplication::render() {
    if (m_outsideWindow.isOpen()) {
        m_outsideWindow.clear();
        m_outsideWindow.draw(m_outsideBackground);
        m_outsideWindow.draw(m_grassRect);
        for (auto& flower : m_flowers) flower->draw(m_outsideWindow);
        for (auto& intruder : m_intruders) intruder->draw(m_outsideWindow);
        for (auto& entity : m_entities) if (entity->getWindowType() == EWindowType::OUTSIDE) entity->draw(m_outsideWindow);
        m_outsideWindow.display();
    }
    if (m_insideWindow.isOpen()) {
        m_insideWindow.clear();
        m_insideWindow.draw(m_insideBackground);
        for (auto& entity : m_entities) if (entity->getWindowType() == EWindowType::BEEHIVE) entity->draw(m_insideWindow);
        m_insideWindow.display();
    }
}

void CApplication::addPollenToHive(int amount) {
    if (amount <= 0) return;
    m_currentPollen += amount;
    std::cout << "XP: " << m_currentPollen << " / " << m_pollenForNextLevel << std::endl;
    if (m_currentPollen >= m_pollenForNextLevel) performLevelUp();
}

void CApplication::performLevelUp() {
    m_hiveLevel++;
    m_currentPollen = 0;
    m_pollenForNextLevel = static_cast<int>(m_pollenForNextLevel * 1.5f);

    std::cout << "=== LEVEL UP: NIVEAU " << m_hiveLevel << " ===" << std::endl;

    int nbWorkers = 0, nbWarriors = 0;
    for (auto& entity : m_entities) {
        CBee* bee = dynamic_cast<CBee*>(entity.get());
        if (bee) {
            bee->levelUpStats(0.1f);
            if (dynamic_cast<CWorkerBee*>(bee)) nbWorkers++;
            else if (dynamic_cast<CWarriorBee*>(bee)) nbWarriors++;
        }
    }

    if (nbWarriors > nbWorkers/2) {
        m_entities.push_back(std::make_unique<CWorkerBee>(sf::Vector2f(350.f, 250.f), EWindowType::BEEHIVE));
        std::cout << "Nouvelle Ouvriere !" << std::endl;
    } else {
        auto newWarrior = std::make_unique<CWarriorBee>(sf::Vector2f(350.f, 250.f), EWindowType::BEEHIVE);
        newWarrior->setHomePosition(sf::Vector2f(450.f, 100.f + (nbWarriors * 40.f)));
        m_entities.push_back(std::move(newWarrior));
        std::cout << "Nouvelle Guerriere !" << std::endl;
    }
}