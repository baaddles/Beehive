#include "Core/CApplication.hpp"
#include "Utils/CConstants.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm>

#ifndef FONTS_PATH
#define FONTS_PATH "assets/fonts/" 
#endif

// ... (Constructeur, initUI, run, handleEvents inchangés : garde ton code actuel) ...
// Pour gagner de la place, je mets juste les méthodes modifiées.

// Copie tout le fichier si tu as un doute, mais voici les parties critiques :

CApplication::CApplication()
    : m_hiveLevel(1),
      m_currentPollen(0),
      m_pollenForNextLevel(constants::POLLEN_FOR_FIRST_LEVEL),
      m_hiveHealth(constants::INITIAL_HIVE_HEALTH), 
      m_hiveMaxHealth(constants::INITIAL_HIVE_HEALTH), 
      m_waveCount(0),
      m_outsideWindow(sf::VideoMode(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT), "Outside - Nature"),
      m_insideWindow(sf::VideoMode(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT), "Inside - Beehive"),
      m_grassRect(sf::Vector2f((float)constants::WINDOW_WIDTH, (float)constants::WINDOW_HEIGHT * constants::GRASS_HEIGHT_RATIO))
{
    m_outsideWindow.setFramerateLimit(60);
    m_insideWindow.setFramerateLimit(60);
    m_insideWindow.setPosition(sf::Vector2i(constants::WINDOW_POS_X, constants::WINDOW_POS_Y));
    m_outsideWindow.setPosition(sf::Vector2i(constants::WINDOW_POS_X + constants::WINDOW_OFFSET, constants::WINDOW_POS_Y));
    
    m_outsideBackground.setTexture(CTextureManager::instance().getTexture("outside_bg.png"));
    m_insideBackground.setTexture(CTextureManager::instance().getTexture("hive_bg.png"));

    float grassHeight = m_grassRect.getSize().y;
    m_grassRect.setFillColor(constants::GRASS_COLOR);
    m_grassRect.setPosition(0.f, (float)constants::WINDOW_HEIGHT - grassHeight);

    for(int i = 0; i < constants::INITIAL_FLOWER_COUNT; ++i) {
        float x = 50.f + (std::rand() % (constants::WINDOW_WIDTH - 100));
        float y = ((float)constants::WINDOW_HEIGHT - grassHeight) + (std::rand() % (int)(grassHeight - 40));
        m_flowers.push_back(std::make_unique<CFlower>(sf::Vector2f(x, y)));
    }
    m_entities.push_back(std::make_unique<CQueenBee>(sf::Vector2f(75.f, 250.f)));
    for(int i = 0; i < constants::INITIAL_WORKER_COUNT; ++i) {
        m_entities.push_back(std::make_unique<CWorkerBee>(sf::Vector2f(100.f + (rand() % 400), 100.f + (rand() % 300)), EWindowType::BEEHIVE));
    }
    for(int i = 0; i < constants::INITIAL_WARRIOR_COUNT; ++i) {
        auto warrior = std::make_unique<CWarriorBee>(sf::Vector2f(350.f, 200.f), EWindowType::BEEHIVE);
        float gridX = constants::GRID_START_X + (i / constants::WARRIORS_PER_COLUMN) * constants::GRID_SPACING_X;
        float gridY = constants::GRID_START_Y + (i % constants::WARRIORS_PER_COLUMN) * constants::GRID_SPACING_Y;
        warrior->setHomePosition(sf::Vector2f(gridX, gridY));
        warrior->setPatrolPosition(warrior->getHomePosition());
        warrior->setPosition(warrior->getHomePosition());
        m_entities.push_back(std::move(warrior));
    }
    initUI();
}

void CApplication::initUI() {
    std::string fontPath = std::string(FONTS_PATH) + "meatball.ttf";
    if (!m_font.loadFromFile(fontPath)) { std::cerr << "ERREUR FATALE FONT" << std::endl; }

    m_txtCounters.setFont(m_font); m_txtCounters.setCharacterSize(16); m_txtCounters.setFillColor(sf::Color::White); m_txtCounters.setPosition(10.f, 10.f);
    m_barHealthBack.setSize(sf::Vector2f(constants::UI_BAR_WIDTH, constants::UI_HEALTH_BAR_HEIGHT)); m_barHealthBack.setFillColor(sf::Color::Black); m_barHealthBack.setOutlineThickness(2.f); m_barHealthBack.setOutlineColor(sf::Color::White); m_barHealthBack.setOrigin(constants::UI_BAR_WIDTH / 2.f, 0.f); m_barHealthBack.setPosition(constants::HIVE_CENTER_X, constants::UI_HEALTH_Y);
    m_barHealthFront.setSize(sf::Vector2f(constants::UI_BAR_WIDTH, constants::UI_HEALTH_BAR_HEIGHT)); m_barHealthFront.setFillColor(sf::Color::Green); m_barHealthFront.setOrigin(constants::UI_BAR_WIDTH / 2.f, 0.f); m_barHealthFront.setPosition(constants::HIVE_CENTER_X, constants::UI_HEALTH_Y);
    m_txtHealthInfo.setFont(m_font); m_txtHealthInfo.setCharacterSize(14); m_txtHealthInfo.setFillColor(sf::Color::White); m_txtHealthInfo.setOutlineColor(sf::Color::Black); m_txtHealthInfo.setOutlineThickness(1.f);
    m_barXPBack.setSize(sf::Vector2f(constants::UI_BAR_WIDTH, constants::UI_XP_BAR_HEIGHT)); m_barXPBack.setFillColor(sf::Color(50, 50, 50)); m_barXPBack.setOrigin(0.f, 0.f); m_barXPBack.setPosition(constants::HIVE_CENTER_X - (constants::UI_BAR_WIDTH / 2.f), constants::UI_XP_Y); 
    m_barXPFront.setSize(sf::Vector2f(0.f, constants::UI_XP_BAR_HEIGHT)); m_barXPFront.setFillColor(sf::Color::Cyan); m_barXPFront.setOrigin(0.f, 0.f); m_barXPFront.setPosition(constants::HIVE_CENTER_X - (constants::UI_BAR_WIDTH / 2.f), constants::UI_XP_Y);
    m_txtLevelInfo.setFont(m_font); m_txtLevelInfo.setCharacterSize(12); m_txtLevelInfo.setFillColor(sf::Color::Black);
    m_txtGameOver.setFont(m_font); m_txtGameOver.setString("GAME OVER"); m_txtGameOver.setCharacterSize(60); m_txtGameOver.setFillColor(sf::Color::Red);
    sf::FloatRect bounds = m_txtGameOver.getLocalBounds(); m_txtGameOver.setOrigin(bounds.width/2.f, bounds.height/2.f); m_txtGameOver.setPosition(constants::HIVE_CENTER_X, constants::HIVE_CENTER_Y);

    m_tooltipText.setFont(m_font);
    m_tooltipText.setCharacterSize(14);
    m_tooltipText.setFillColor(sf::Color::White);
    m_tooltipBox.setFillColor(sf::Color(0, 0, 0, 200));
    m_tooltipBox.setOutlineColor(sf::Color::White);
    m_tooltipBox.setOutlineThickness(1.f);
}

void CApplication::drawTooltip(sf::RenderWindow& window, const std::string& textStr, sf::Vector2f mousePos) {
    if (textStr.empty()) return;
    m_tooltipText.setString(textStr);
    sf::FloatRect textBounds = m_tooltipText.getLocalBounds();
    float padding = 5.f;
    m_tooltipBox.setSize(sf::Vector2f(textBounds.width + padding * 2.f, textBounds.height + padding * 2.f + 5.f));
    sf::Vector2f tooltipPos = mousePos + sf::Vector2f(15.f, 15.f);
    if (tooltipPos.x + m_tooltipBox.getSize().x > window.getSize().x) { tooltipPos.x = mousePos.x - m_tooltipBox.getSize().x - 5.f; }
    m_tooltipBox.setPosition(tooltipPos);
    m_tooltipText.setPosition(tooltipPos.x + padding, tooltipPos.y + padding);
    window.draw(m_tooltipBox);
    window.draw(m_tooltipText);
}

void CApplication::run() { sf::Clock clock; while (m_outsideWindow.isOpen() || m_insideWindow.isOpen()) { float dt = clock.restart().asSeconds(); handleEvents(); update(dt); render(); } }
void CApplication::handleEvents() { sf::Event event; if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { m_outsideWindow.close(); m_insideWindow.close(); } while (m_outsideWindow.pollEvent(event)) if (event.type == sf::Event::Closed) m_outsideWindow.close(); while (m_insideWindow.pollEvent(event)) if (event.type == sf::Event::Closed) m_insideWindow.close(); }

// --- METHODE UPDATE CORRIGÉE ---
void CApplication::update(float dt) {
    if (m_hiveHealth <= 0) return;
    for (auto& entity : m_entities) {
        CQueenBee* queen = dynamic_cast<CQueenBee*>(entity.get());
        if (queen) { queen->setHiveLevel(m_hiveLevel); if (queen->isReadyToSpawn()) { spawnUnit(); queen->resetSpawnTimer(); } break; }
    }
    
    // Correction Fleurs : passage de m_outsideWindow.getSize() ou (0,0) car pas utilisé
    int availableFlowers = 0; 
    for (auto& flower : m_flowers) { 
        flower->update(dt, m_outsideWindow.getSize()); 
        if (flower->isAvailable()) availableFlowers++; 
    }
    
    bool isUnderAttack = !m_intruders.empty(); 
    CIntruder* targetEnnemy = nullptr; 
    if (isUnderAttack) targetEnnemy = m_intruders[0].get();

    for (auto& entity : m_entities) {
        EWindowType currentWin = entity->getWindowType(); 
        sf::Vector2u winSize = (currentWin == EWindowType::BEEHIVE) ? m_insideWindow.getSize() : m_outsideWindow.getSize();
        entity->update(dt, winSize);
        // ... (Logique Workers/Warriors inchangée) ...
        float currentSpeed = 50.f; CBee* bee = dynamic_cast<CBee*>(entity.get()); if (bee) currentSpeed = bee->getSpeed();
        CWorkerBee* worker = dynamic_cast<CWorkerBee*>(entity.get());
        if (worker) {
            if (isUnderAttack) {
                if (currentWin == EWindowType::OUTSIDE) { sf::Vector2f pos = worker->getPosition(); pos.x -= (currentSpeed * 1.5f) * dt; if (pos.x <= 5.f) { worker->setWindowType(EWindowType::BEEHIVE); worker->setPosition(sf::Vector2f((float)constants::WINDOW_WIDTH - 15.f, pos.y)); worker->startDelivering(); } else worker->setPosition(pos); }
                else if (currentWin == EWindowType::BEEHIVE && worker->getBehavior() == EWorkerBehavior::DELIVERING) { sf::Vector2f diff = sf::Vector2f(constants::HIVE_CENTER_X, constants::HIVE_CENTER_Y) - worker->getPosition(); if ((diff.x*diff.x+diff.y*diff.y) < 400.f) { addPollenToHive(worker->getPollenCollected()); worker->resetPollen(); } }
            } else {
                if (currentWin == EWindowType::OUTSIDE && !worker->isFull() && worker->getBehavior() == EWorkerBehavior::WANDERING) worker->searchFlower(m_flowers);
                if (currentWin == EWindowType::BEEHIVE && !worker->isFull() && worker->getBehavior() == EWorkerBehavior::WANDERING && availableFlowers > 0) { sf::Vector2f pos = worker->getPosition(); worker->setPosition(sf::Vector2f(pos.x + currentSpeed * dt, pos.y)); if (pos.x >= (float)constants::WINDOW_WIDTH - 5.f) { worker->setWindowType(EWindowType::OUTSIDE); worker->setPosition(sf::Vector2f(10.f, pos.y)); } }
                else if (currentWin == EWindowType::OUTSIDE && (worker->isFull() || worker->getBehavior() == EWorkerBehavior::RETURNING)) { sf::Vector2f pos = worker->getPosition(); if (pos.x <= 5.f) { worker->setWindowType(EWindowType::BEEHIVE); worker->setPosition(sf::Vector2f((float)constants::WINDOW_WIDTH - 15.f, pos.y)); worker->startDelivering(); } }
                else if (currentWin == EWindowType::BEEHIVE && worker->getBehavior() == EWorkerBehavior::DELIVERING) { sf::Vector2f diff = sf::Vector2f(constants::HIVE_CENTER_X, constants::HIVE_CENTER_Y) - worker->getPosition(); if ((diff.x*diff.x+diff.y*diff.y) < 400.f) { addPollenToHive(worker->getPollenCollected()); worker->resetPollen(); } }
            }
        }
        CWarriorBee* warrior = dynamic_cast<CWarriorBee*>(entity.get());
        if (warrior) {
            if (isUnderAttack && targetEnnemy) {
                if (currentWin == EWindowType::BEEHIVE) warrior->setHomePosition(sf::Vector2f(2000.f, targetEnnemy->getPosition().y)); else warrior->setHomePosition(targetEnnemy->getPosition());
                if (currentWin == EWindowType::OUTSIDE && !targetEnnemy->isDead()) { if (warrior->canAttack() && warrior->getBounds().intersects(targetEnnemy->getBounds())) { targetEnnemy->takeDamage(warrior->getForce()); warrior->resetAttackTimer(); sf::Vector2f dir = targetEnnemy->getPosition() - warrior->getPosition(); warrior->applyKnockback(-dir, 120.f); } }
                else if (currentWin == EWindowType::BEEHIVE) { sf::Vector2f pos = warrior->getPosition(); if (pos.x >= (float)constants::WINDOW_WIDTH - 5.f) { warrior->setWindowType(EWindowType::OUTSIDE); warrior->setPosition(sf::Vector2f(10.f, pos.y)); } }
            } else {
                warrior->setHomePosition(warrior->getPatrolPosition());
                if (currentWin == EWindowType::OUTSIDE) { warrior->setHomePosition(sf::Vector2f(-500.f, warrior->getPatrolPosition().y)); sf::Vector2f pos = warrior->getPosition(); if (pos.x <= 5.f) { warrior->setWindowType(EWindowType::BEEHIVE); warrior->setPosition(sf::Vector2f((float)constants::WINDOW_WIDTH - 15.f, pos.y)); } }
            }
        }
    }
    
    // --- CORRECTION INTRUS : Ajout de la taille fenetre ---
    for (auto it = m_intruders.begin(); it != m_intruders.end(); ) { 
        // ICI : on passe m_outsideWindow.getSize()
        (*it)->update(dt, m_outsideWindow.getSize()); 
        
        bool removed = false; 
        if ((*it)->isDead()) removed = true; 
        else if ((*it)->getPosition().x <= 0.f) { m_hiveHealth -= (*it)->getForce(); removed = true; } 
        if (removed) it = m_intruders.erase(it); else ++it; 
    }
}

// ... (render, addPollen, spawnWave, spawnUnit, performLevelUp inchangés) ...
void CApplication::render() {
    if (m_outsideWindow.isOpen()) {
        m_outsideWindow.clear();
        m_outsideWindow.draw(m_outsideBackground);
        m_outsideWindow.draw(m_grassRect);
        for (auto& flower : m_flowers) flower->draw(m_outsideWindow);
        for (auto& intruder : m_intruders) intruder->draw(m_outsideWindow);
        for (auto& entity : m_entities) if (entity->getWindowType() == EWindowType::OUTSIDE) entity->draw(m_outsideWindow);
        sf::Vector2i mousePix = sf::Mouse::getPosition(m_outsideWindow);
        sf::Vector2f mousePos = m_outsideWindow.mapPixelToCoords(mousePix);
        for (auto& flower : m_flowers) if (flower->getBounds().contains(mousePos)) drawTooltip(m_outsideWindow, flower->getStats(), mousePos);
        for (auto& intruder : m_intruders) if (intruder->getBounds().contains(mousePos)) drawTooltip(m_outsideWindow, intruder->getStats(), mousePos);
        for (auto& entity : m_entities) if (entity->getWindowType() == EWindowType::OUTSIDE && entity->getBounds().contains(mousePos)) drawTooltip(m_outsideWindow, entity->getStats(), mousePos);
        m_outsideWindow.display();
    }
    if (m_insideWindow.isOpen()) {
        m_insideWindow.clear();
        m_insideWindow.draw(m_insideBackground);
        for (auto& entity : m_entities) if (entity->getWindowType() == EWindowType::BEEHIVE) entity->draw(m_insideWindow);
        renderUI(m_insideWindow);
        sf::Vector2i mousePix = sf::Mouse::getPosition(m_insideWindow);
        sf::Vector2f mousePos = m_insideWindow.mapPixelToCoords(mousePix);
        for (auto& entity : m_entities) { if (entity->getWindowType() == EWindowType::BEEHIVE && entity->getBounds().contains(mousePos)) { drawTooltip(m_insideWindow, entity->getStats(), mousePos); } }
        m_insideWindow.display();
    }
}
void CApplication::renderUI(sf::RenderWindow& window) { if (m_hiveHealth <= 0) { window.draw(m_txtGameOver); return; } int nbW = 0, nbWar = 0; for(auto& e : m_entities) { if(dynamic_cast<CWorkerBee*>(e.get())) nbW++; if(dynamic_cast<CWarriorBee*>(e.get())) nbWar++; } std::stringstream ss; ss << "Workers: " << nbW << "\nWarriors: " << nbWar; m_txtCounters.setString(ss.str()); float hpRatio = (float)m_hiveHealth / (float)m_hiveMaxHealth; if (hpRatio < 0.f) hpRatio = 0.f; m_barHealthFront.setSize(sf::Vector2f(constants::UI_BAR_WIDTH * hpRatio, constants::UI_HEALTH_BAR_HEIGHT)); if (hpRatio > 0.5f) m_barHealthFront.setFillColor(sf::Color::Green); else if (hpRatio > 0.25f) m_barHealthFront.setFillColor(sf::Color::Yellow); else if (hpRatio > 0.10f) m_barHealthFront.setFillColor(sf::Color(255, 165, 0)); else m_barHealthFront.setFillColor(sf::Color::Red); std::stringstream ssHp; ssHp << "HP: " << m_hiveHealth << " / " << m_hiveMaxHealth; m_txtHealthInfo.setString(ssHp.str()); sf::FloatRect hpRect = m_txtHealthInfo.getLocalBounds(); m_txtHealthInfo.setOrigin(hpRect.left + hpRect.width/2.0f, hpRect.top + hpRect.height/2.0f); m_txtHealthInfo.setPosition(constants::HIVE_CENTER_X, constants::UI_HEALTH_Y + constants::UI_HEALTH_BAR_HEIGHT/2.f); float xpRatio = (float)m_currentPollen / (float)m_pollenForNextLevel; if (xpRatio > 1.f) xpRatio = 1.f; m_barXPFront.setSize(sf::Vector2f(constants::UI_BAR_WIDTH * xpRatio, constants::UI_XP_BAR_HEIGHT)); std::stringstream ssLvl; ssLvl << "Lvl " << m_hiveLevel << " - " << m_currentPollen << " / " << m_pollenForNextLevel; m_txtLevelInfo.setString(ssLvl.str()); sf::FloatRect xpRect = m_txtLevelInfo.getLocalBounds(); m_txtLevelInfo.setOrigin(xpRect.left + xpRect.width/2.0f, xpRect.top + xpRect.height/2.0f); m_txtLevelInfo.setPosition(constants::HIVE_CENTER_X, constants::UI_XP_Y + constants::UI_XP_BAR_HEIGHT/2.f); window.draw(m_barHealthBack); window.draw(m_barHealthFront); window.draw(m_txtHealthInfo); window.draw(m_barXPBack); window.draw(m_barXPFront); window.draw(m_txtLevelInfo); window.draw(m_txtCounters); }
void CApplication::addPollenToHive(int amount) { if (amount <= 0) return; m_currentPollen += amount; if (m_hiveHealth < m_hiveMaxHealth) { m_hiveHealth += 1; if (m_hiveHealth > m_hiveMaxHealth) m_hiveHealth = m_hiveMaxHealth; } if (m_currentPollen >= m_pollenForNextLevel) performLevelUp(); }
void CApplication::spawnWave() { m_waveCount++; std::cout << "!!! VAGUE " << m_waveCount << " !!!" << std::endl; float multiplier = 1.0f + (m_waveCount * constants::WAVE_HP_FORCE_MULT); int finalHP = static_cast<int>(constants::BASE_INTRUDER_HP * multiplier); int finalForce = static_cast<int>(constants::BASE_INTRUDER_FORCE * multiplier); float finalSpeed = constants::BASE_INTRUDER_SPEED * (2.0f + (m_waveCount * constants::WAVE_SPEED_MULT)); for(int i = 0; i < constants::INTRUDERS_PER_WAVE; ++i) { float startY = 100.f + (std::rand() % 300); float startX = (float)constants::WINDOW_WIDTH + (i * 60.f); m_intruders.push_back(std::make_unique<CIntruder>(sf::Vector2f(startX, startY), finalHP, finalForce, finalSpeed)); } }
void CApplication::spawnUnit() { int nbWorkers = 0, nbWarriors = 0; for (auto& entity : m_entities) { if (dynamic_cast<CWorkerBee*>(entity.get())) nbWorkers++; if (dynamic_cast<CWarriorBee*>(entity.get())) nbWarriors++; } if (nbWarriors > nbWorkers) { m_entities.push_back(std::make_unique<CWorkerBee>(sf::Vector2f(350.f, 250.f), EWindowType::BEEHIVE)); std::cout << "[REINE] Naissance d'une Ouvriere" << std::endl; } else { auto newWarrior = std::make_unique<CWarriorBee>(sf::Vector2f(350.f, 250.f), EWindowType::BEEHIVE); float gridX = constants::GRID_START_X + (nbWarriors / constants::WARRIORS_PER_COLUMN) * constants::GRID_SPACING_X; float gridY = constants::GRID_START_Y + (nbWarriors % constants::WARRIORS_PER_COLUMN) * constants::GRID_SPACING_Y; newWarrior->setHomePosition(sf::Vector2f(gridX, gridY)); newWarrior->setPatrolPosition(newWarrior->getHomePosition()); m_entities.push_back(std::move(newWarrior)); std::cout << "[REINE] Naissance d'une Guerriere" << std::endl; } }
void CApplication::performLevelUp() { m_hiveLevel++; m_currentPollen = 0; m_pollenForNextLevel = static_cast<int>(m_pollenForNextLevel * 1.5f); int maxHealthGain = 20; m_hiveMaxHealth += maxHealthGain; m_hiveHealth += maxHealthGain; int healAmount = static_cast<int>(m_hiveMaxHealth * constants::LEVEL_UP_HEAL_RATIO); m_hiveHealth = std::min(m_hiveHealth + healAmount, m_hiveMaxHealth); for (auto& entity : m_entities) { CBee* bee = dynamic_cast<CBee*>(entity.get()); if (bee) { bee->levelUpStats(constants::LEVEL_UP_STAT_BUFF); } } std::cout << "=== LEVEL UP: NIVEAU " << m_hiveLevel << " ===" << std::endl; spawnWave(); }