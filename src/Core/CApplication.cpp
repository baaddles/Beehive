#include "Core/CApplication.hpp"
#include "Utils/CConstants.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm> // Pour std::min

CApplication::CApplication()
    : m_hiveLevel(1),
      m_currentPollen(0),
      m_pollenForNextLevel(50),
      m_hiveHealth(100), m_hiveMaxHealth(100), m_waveCount(0),
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

    // Initialisation Fleurs
    for(int i = 0; i < constants::INITIAL_FLOWER_COUNT; ++i) {
        float x = 50.f + (std::rand() % 600);
        float y = (500.f - grassHeight) + (std::rand() % (int)(grassHeight - 40));
        m_flowers.push_back(std::make_unique<CFlower>(sf::Vector2f(x, y)));
    }

    // Reine
    m_entities.push_back(std::make_unique<CQueenBee>(sf::Vector2f(75.f, 250.f)));

    // Ouvrières initiales
    for(int i = 0; i < 5; ++i) {
        m_entities.push_back(std::make_unique<CWorkerBee>(
            sf::Vector2f(100.f + (rand() % 400), 100.f + (rand() % 300)), EWindowType::BEEHIVE));
    }

    // Guerrières initiales (GRILLE)
    for(int i = 0; i < 5; ++i) {
        auto warrior = std::make_unique<CWarriorBee>(sf::Vector2f(350.f, 200.f), EWindowType::BEEHIVE);
        
        float gridX = 450.f + (i / 5) * 50.f; 
        float gridY = 100.f + (i % 5) * 60.f;

        warrior->setHomePosition(sf::Vector2f(gridX, gridY));
        warrior->setPatrolPosition(warrior->getHomePosition());
        warrior->setPosition(warrior->getHomePosition());
        m_entities.push_back(std::move(warrior));
    }

    initUI();
}

void CApplication::initUI() {
    // Construction du chemin absolu via la macro CMake
    // FONTS_PATH se termine déjà par un '/' dans ton CMakeLists.txt
    std::string fontPath = std::string(FONTS_PATH) + "meatball.ttf";

    if (!m_font.loadFromFile(fontPath)) {
        std::cerr << "ERREUR FATALE : Impossible de charger la police : " << fontPath << std::endl;
        std::cerr << "Verifiez que le dossier 'assets/fonts/' existe et contient 'arial.ttf'" << std::endl;
        // Tu peux ajouter un fallback système ici si tu veux, mais c'est mieux de fixer l'asset.
    }

    // --- 1. Compteurs (Haut Gauche) ---
    m_txtCounters.setFont(m_font);
    m_txtCounters.setCharacterSize(16);
    m_txtCounters.setFillColor(sf::Color::White);
    m_txtCounters.setPosition(10.f, 10.f);

    // --- 2. Barre de Vie (Bas Milieu) ---
    // Fond
    m_barHealthBack.setSize(sf::Vector2f(300.f, 24.f));
    m_barHealthBack.setFillColor(sf::Color::Black);
    m_barHealthBack.setOutlineThickness(2.f);
    m_barHealthBack.setOutlineColor(sf::Color::White);
    m_barHealthBack.setOrigin(150.f, 0.f); // Origine au milieu horizontal
    m_barHealthBack.setPosition(350.f, 420.f);

    // Devant (Vert/Rouge)
    m_barHealthFront.setSize(sf::Vector2f(300.f, 24.f));
    m_barHealthFront.setFillColor(sf::Color::Green);
    m_barHealthFront.setOrigin(150.f, 0.f);
    m_barHealthFront.setPosition(350.f, 420.f);

    // Texte Vie
    m_txtHealthInfo.setFont(m_font);
    m_txtHealthInfo.setCharacterSize(14);
    m_txtHealthInfo.setFillColor(sf::Color::White);
    m_txtHealthInfo.setOutlineColor(sf::Color::Black);
    m_txtHealthInfo.setOutlineThickness(1.f);
    // La position exacte est recalculée dans renderUI pour le centrage

    // --- 3. Barre XP (Sous la vie) ---
    // Fond
    m_barXPBack.setSize(sf::Vector2f(300.f, 16.f));
    m_barXPBack.setFillColor(sf::Color(50, 50, 50));
    m_barXPBack.setOrigin(0.f, 0.f); // Origine à gauche pour faciliter le calcul
    m_barXPBack.setPosition(200.f, 455.f); // 350 - 150 = 200

    // Devant (Bleu)
    m_barXPFront.setSize(sf::Vector2f(0.f, 16.f));
    m_barXPFront.setFillColor(sf::Color::Cyan);
    m_barXPFront.setOrigin(0.f, 0.f);
    m_barXPFront.setPosition(200.f, 455.f);

    // Texte XP
    m_txtLevelInfo.setFont(m_font);
    m_txtLevelInfo.setCharacterSize(12);
    m_txtLevelInfo.setFillColor(sf::Color::Black);
    // La position exacte est recalculée dans renderUI

    // --- 4. Game Over ---
    m_txtGameOver.setFont(m_font);
    m_txtGameOver.setString("GAME OVER");
    m_txtGameOver.setCharacterSize(60);
    m_txtGameOver.setFillColor(sf::Color::Red);
    
    // Centrage du Game Over
    sf::FloatRect bounds = m_txtGameOver.getLocalBounds();
    m_txtGameOver.setOrigin(bounds.width/2.f, bounds.height/2.f);
    m_txtGameOver.setPosition(350.f, 250.f);
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { m_outsideWindow.close(); m_insideWindow.close(); }
    while (m_outsideWindow.pollEvent(event)) if (event.type == sf::Event::Closed) m_outsideWindow.close();
    while (m_insideWindow.pollEvent(event)) if (event.type == sf::Event::Closed) m_insideWindow.close();
}

void CApplication::update(float dt) {
    if (m_hiveHealth <= 0) return;

    int availableFlowers = 0;
    for (auto& flower : m_flowers) { flower->update(dt); if (flower->isAvailable()) availableFlowers++; }

    bool isUnderAttack = !m_intruders.empty();
    CIntruder* targetEnnemy = nullptr;
    if (isUnderAttack) targetEnnemy = m_intruders[0].get();

    for (auto& entity : m_entities) {
        EWindowType currentWin = entity->getWindowType();
        sf::Vector2u winSize = (currentWin == EWindowType::BEEHIVE) ? m_insideWindow.getSize() : m_outsideWindow.getSize();
        entity->update(dt, winSize);

        float currentSpeed = 50.f;
        CBee* bee = dynamic_cast<CBee*>(entity.get());
        if (bee) currentSpeed = bee->getSpeed();

        // --- WORKER ---
        CWorkerBee* worker = dynamic_cast<CWorkerBee*>(entity.get());
        if (worker) {
            if (isUnderAttack) {
                if (currentWin == EWindowType::OUTSIDE) {
                    sf::Vector2f pos = worker->getPosition();
                    pos.x -= (currentSpeed * 1.5f) * dt;
                    if (pos.x <= 5.f) {
                        worker->setWindowType(EWindowType::BEEHIVE);
                        worker->setPosition(sf::Vector2f(685.f, pos.y));
                        worker->startDelivering();
                    } else worker->setPosition(pos);
                }
                else if (currentWin == EWindowType::BEEHIVE) {
                   if (worker->getBehavior() == EWorkerBehavior::DELIVERING) {
                        sf::Vector2f diff = sf::Vector2f(constants::HIVE_CENTER_X, constants::HIVE_CENTER_Y) - worker->getPosition();
                        float distSq = diff.x * diff.x + diff.y * diff.y;
                        if (distSq < 400.f) { addPollenToHive(worker->getPollenCollected()); worker->resetPollen(); }
                   }
                }
            } else {
                if (currentWin == EWindowType::OUTSIDE && !worker->isFull() && worker->getBehavior() == EWorkerBehavior::WANDERING) worker->searchFlower(m_flowers);
                
                if (currentWin == EWindowType::BEEHIVE && !worker->isFull() && worker->getBehavior() == EWorkerBehavior::WANDERING && availableFlowers > 0) {
                    sf::Vector2f pos = worker->getPosition();
                    worker->setPosition(sf::Vector2f(pos.x + currentSpeed * dt, pos.y));
                    if (pos.x >= 695.f) { worker->setWindowType(EWindowType::OUTSIDE); worker->setPosition(sf::Vector2f(10.f, pos.y)); }
                }
                else if (currentWin == EWindowType::OUTSIDE && (worker->isFull() || worker->getBehavior() == EWorkerBehavior::RETURNING)) {
                    sf::Vector2f pos = worker->getPosition();
                    if (pos.x <= 5.f) { worker->setWindowType(EWindowType::BEEHIVE); worker->setPosition(sf::Vector2f(685.f, pos.y)); worker->startDelivering(); }
                }
                else if (currentWin == EWindowType::BEEHIVE && worker->getBehavior() == EWorkerBehavior::DELIVERING) {
                    sf::Vector2f diff = sf::Vector2f(constants::HIVE_CENTER_X, constants::HIVE_CENTER_Y) - worker->getPosition();
                    float distSq = diff.x * diff.x + diff.y * diff.y;
                    if (distSq < 400.f) { addPollenToHive(worker->getPollenCollected()); worker->resetPollen(); }
                }
            }
        }

        // --- WARRIOR ---
        CWarriorBee* warrior = dynamic_cast<CWarriorBee*>(entity.get());
        if (warrior) {
            if (isUnderAttack && targetEnnemy) {
                if (currentWin == EWindowType::BEEHIVE) warrior->setHomePosition(sf::Vector2f(2000.f, targetEnnemy->getPosition().y));
                else warrior->setHomePosition(targetEnnemy->getPosition());

                if (currentWin == EWindowType::OUTSIDE && !targetEnnemy->isDead()) {
                    if (warrior->canAttack() && warrior->getBounds().intersects(targetEnnemy->getBounds())) {
                        targetEnnemy->takeDamage(warrior->getForce());
                        warrior->resetAttackTimer();

                        sf::Vector2f dir = targetEnnemy->getPosition() - warrior->getPosition();
                        warrior->applyKnockback(-dir, 120.f); // RECUL REDUIT (120)
                    }
                }
                else if (currentWin == EWindowType::BEEHIVE) {
                    sf::Vector2f pos = warrior->getPosition();
                    if (pos.x >= 695.f) { warrior->setWindowType(EWindowType::OUTSIDE); warrior->setPosition(sf::Vector2f(10.f, pos.y)); }
                }
            } else {
                warrior->setHomePosition(warrior->getPatrolPosition());
                if (currentWin == EWindowType::OUTSIDE) {
                    warrior->setHomePosition(sf::Vector2f(-500.f, warrior->getPatrolPosition().y));
                    sf::Vector2f pos = warrior->getPosition();
                    if (pos.x <= 5.f) { warrior->setWindowType(EWindowType::BEEHIVE); warrior->setPosition(sf::Vector2f(685.f, pos.y)); }
                }
            }
        }
    }

    for (auto it = m_intruders.begin(); it != m_intruders.end(); ) {
        (*it)->update(dt);
        bool removed = false;
        if ((*it)->isDead()) removed = true;
        else if ((*it)->getPosition().x <= 0.f) { m_hiveHealth -= (*it)->getForce(); removed = true; }
        if (removed) it = m_intruders.erase(it); else ++it;
    }
}

void CApplication::renderUI(sf::RenderWindow& window) {
    if (m_hiveHealth <= 0) {
        window.draw(m_txtGameOver);
        return;
    }

    int nbW = 0, nbWar = 0;
    for(auto& e : m_entities) {
        if(dynamic_cast<CWorkerBee*>(e.get())) nbW++;
        if(dynamic_cast<CWarriorBee*>(e.get())) nbWar++;
    }
    std::stringstream ss;
    ss << "Workers: " << nbW << "\nWarriors: " << nbWar;
    m_txtCounters.setString(ss.str());

    float hpRatio = (float)m_hiveHealth / (float)m_hiveMaxHealth;
    if (hpRatio < 0.f) hpRatio = 0.f;
    m_barHealthFront.setSize(sf::Vector2f(300.f * hpRatio, 24.f));

    if (hpRatio > 0.5f) m_barHealthFront.setFillColor(sf::Color::Green);
    else if (hpRatio > 0.25f) m_barHealthFront.setFillColor(sf::Color::Yellow);
    else if (hpRatio > 0.10f) m_barHealthFront.setFillColor(sf::Color(255, 165, 0));
    else m_barHealthFront.setFillColor(sf::Color::Red);

    std::stringstream ssHp;
    ssHp << "HP: " << m_hiveHealth << " / " << m_hiveMaxHealth;
    m_txtHealthInfo.setString(ssHp.str());
    sf::FloatRect hpRect = m_txtHealthInfo.getLocalBounds();
    m_txtHealthInfo.setOrigin(hpRect.left + hpRect.width/2.0f, hpRect.top + hpRect.height/2.0f);
    m_txtHealthInfo.setPosition(350.f, 420.f + 12.f); 

    float xpRatio = (float)m_currentPollen / (float)m_pollenForNextLevel;
    if (xpRatio > 1.f) xpRatio = 1.f;
    m_barXPFront.setSize(sf::Vector2f(300.f * xpRatio, 16.f));

    std::stringstream ssLvl;
    ssLvl << "Lvl " << m_hiveLevel << " - " << m_currentPollen << " / " << m_pollenForNextLevel;
    m_txtLevelInfo.setString(ssLvl.str());
    sf::FloatRect xpRect = m_txtLevelInfo.getLocalBounds();
    m_txtLevelInfo.setOrigin(xpRect.left + xpRect.width/2.0f, xpRect.top + xpRect.height/2.0f);
    m_txtLevelInfo.setPosition(350.f, 455.f + 8.f); 

    window.draw(m_barHealthBack);
    window.draw(m_barHealthFront);
    window.draw(m_txtHealthInfo);

    window.draw(m_barXPBack);
    window.draw(m_barXPFront);
    window.draw(m_txtLevelInfo); 

    window.draw(m_txtCounters);
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
        renderUI(m_insideWindow);
        m_insideWindow.display();
    }
}

void CApplication::addPollenToHive(int amount) {
    if (amount <= 0) return;
    m_currentPollen += amount;
    
    // --- SOIN FIXE ---
    if (m_hiveHealth < m_hiveMaxHealth) {
        m_hiveHealth += 2; // +2 HP seulement
        if (m_hiveHealth > m_hiveMaxHealth) m_hiveHealth = m_hiveMaxHealth;
    }
    
    if (m_currentPollen >= m_pollenForNextLevel) performLevelUp();
}

void CApplication::spawnWave() {
    m_waveCount++;
    std::cout << "!!! VAGUE " << m_waveCount << " !!!" << std::endl;

    float multiplier = 1.0f + (m_waveCount * 0.2f);
    
    // --- STATS EQUILIBRÉES (RALENTIES MAIS TANKY) ---
    // PV : 250 (Moins que Hard Mode 350, mais OK vu qu'ils sont lents)
    int baseHP = 250; 
    int finalHP = static_cast<int>(baseHP * multiplier);
    
    // Force : 15 (Mais augmente avec les vagues)
    int baseForce = 15; 
    int finalForce = static_cast<int>(baseForce * multiplier);

    // Vitesse : 35 (Slower than 50, Faster than 23)
    float baseSpeed = 35.0f;
    float finalSpeed = baseSpeed * (1.0f + (m_waveCount * 0.05f));

    for(int i = 0; i < 3; ++i) {
        float startY = 100.f + (std::rand() % 300);
        float startX = 700.f + (i * 60.f); 
        m_intruders.push_back(std::make_unique<CIntruder>(sf::Vector2f(startX, startY), finalHP, finalForce, finalSpeed));
    }
}

void CApplication::performLevelUp() {
    m_hiveLevel++;
    m_currentPollen = 0;
    m_pollenForNextLevel = static_cast<int>(m_pollenForNextLevel * 1.5f);
    
    int maxHealthGain = 20;
    m_hiveMaxHealth += maxHealthGain;
    
    // --- CORRECTION VISUELLE ET SOIN 10% ---
    // 1. On donne les PV correspondant à l'augmentation du Max pour ne pas "perdre" visuellement
    m_hiveHealth += maxHealthGain;

    // 2. On ajoute le soin de 10% (Bonus de Level Up)
    int healAmount = static_cast<int>(m_hiveMaxHealth * 0.10f); // 10%
    m_hiveHealth = std::min(m_hiveHealth + healAmount, m_hiveMaxHealth);

    int nbWorkers = 0, nbWarriors = 0;
    for (auto& entity : m_entities) {
        CBee* bee = dynamic_cast<CBee*>(entity.get());
        if (bee) {
            bee->levelUpStats(0.1f);
            if (dynamic_cast<CWorkerBee*>(bee)) nbWorkers++;
            else if (dynamic_cast<CWarriorBee*>(bee)) nbWarriors++;
        }
    }

    if (nbWarriors > nbWorkers) {
        m_entities.push_back(std::make_unique<CWorkerBee>(sf::Vector2f(350.f, 250.f), EWindowType::BEEHIVE));
        std::cout << "+1 Ouvriere" << std::endl;
    } else {
        auto newWarrior = std::make_unique<CWarriorBee>(sf::Vector2f(350.f, 250.f), EWindowType::BEEHIVE);
        
        float gridX = 450.f + (nbWarriors / 5) * 50.f;
        float gridY = 100.f + (nbWarriors % 5) * 60.f;

        newWarrior->setHomePosition(sf::Vector2f(gridX, gridY));
        newWarrior->setPatrolPosition(newWarrior->getHomePosition());
        
        m_entities.push_back(std::move(newWarrior));
        std::cout << "+1 Guerriere" << std::endl;
    }

    spawnWave();
}