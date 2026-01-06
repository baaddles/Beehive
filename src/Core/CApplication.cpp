#include "Core/CApplication.hpp"
#include <iostream>

CApplication::CApplication()
    : m_outsideWindow(sf::VideoMode(700, 500), "Outside - Nature"),
      m_insideWindow(sf::VideoMode(700, 500), "Inside - Beehive")
{
    m_outsideWindow.setFramerateLimit(60);
    m_insideWindow.setFramerateLimit(60);

    // Inversion des positions X demandée
    m_insideWindow.setPosition(sf::Vector2i(100, 100));
    m_outsideWindow.setPosition(sf::Vector2i(820, 100));

    m_outsideBackground.setTexture(CTextureManager::instance().getTexture("outside_bg.png"));
    m_insideBackground.setTexture(CTextureManager::instance().getTexture("hive_bg.png"));

    // --- INITIALISATION DE LA COLONIE ---
    
    // 1 Reine dans la ruche
    m_entities.push_back(std::make_unique<CQueenBee>(sf::Vector2f(350.f, 250.f)));

    // 10 Ouvrières dans la ruche au départ
    for(int i = 0; i < 10; ++i) {
        m_entities.push_back(std::make_unique<CWorkerBee>(
            sf::Vector2f(100.f + (rand() % 400), 100.f + (rand() % 300)),
            EWindowType::BEEHIVE
        ));
    }

    // 3 Guerrières dans la ruche
    for(int i = 0; i < 3; ++i) {
        m_entities.push_back(std::make_unique<CWarriorBee>(
            sf::Vector2f(200.f + (rand() % 200), 200.f + (rand() % 100))
        ));
    }
}

void CApplication::run()
{
    sf::Clock clock;
    while (m_outsideWindow.isOpen() || m_insideWindow.isOpen())
    {
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}

void CApplication::handleEvents()
{
    sf::Event event;
    while (m_outsideWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) m_outsideWindow.close();
    }
    while (m_insideWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) m_insideWindow.close();
    }
}

void CApplication::update(float dt)
{
    for (auto& entity : m_entities)
    {
        EWindowType currentWin = entity->getWindowType();
        sf::Vector2u winSize = (currentWin == EWindowType::BEEHIVE)
                                ? m_insideWindow.getSize()
                                : m_outsideWindow.getSize();
        
        entity->update(dt, winSize);

        // LOGIQUE DE TRANSFERT
        sf::Vector2f pos = entity->getPosition();

        // Si une ouvrière vide touche le bord gauche de la RUCHE -> elle sort
        if (currentWin == EWindowType::BEEHIVE && pos.x <= 0) {
            entity->setWindowType(EWindowType::OUTSIDE);
            entity->setPosition(sf::Vector2f(winSize.x - 20, pos.y));
        }
        // Si une ouvrière pleine touche le bord droit de l'EXTÉRIEUR -> elle rentre
        else if (currentWin == EWindowType::OUTSIDE && pos.x >= winSize.x - 30) {
            entity->setWindowType(EWindowType::BEEHIVE);
            entity->setPosition(sf::Vector2f(20, pos.y));
            
            // Simulation : elle dépose son pollen en rentrant (pour le test)
            CWorkerBee* worker = dynamic_cast<CWorkerBee*>(entity.get());
            if (worker) worker->setHasPollen(false);
        }
    }
}

void CApplication::render()
{
    if (m_outsideWindow.isOpen()) {
        m_outsideWindow.clear();
        m_outsideWindow.draw(m_outsideBackground);
        for (auto& entity : m_entities) {
            if (entity->getWindowType() == EWindowType::OUTSIDE) entity->draw(m_outsideWindow);
        }
        m_outsideWindow.display();
    }

    if (m_insideWindow.isOpen()) {
        m_insideWindow.clear();
        m_insideWindow.draw(m_insideBackground);
        for (auto& entity : m_entities) {
            if (entity->getWindowType() == EWindowType::BEEHIVE) entity->draw(m_insideWindow);
        }
        m_insideWindow.display();
    }
}