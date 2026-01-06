#include "Core/CApplication.hpp"
#include "Utils/CConstants.hpp"
#include <iostream>

CApplication::CApplication()
    : m_outsideWindow(sf::VideoMode(700, 500), "Outside - Nature"),
      m_insideWindow(sf::VideoMode(700, 500), "Inside - Beehive")
{
    m_outsideWindow.setFramerateLimit(60);
    m_insideWindow.setFramerateLimit(60);

    // Fenêtres côte à côte (inversées selon ta demande)
    m_insideWindow.setPosition(sf::Vector2i(100, 100));
    m_outsideWindow.setPosition(sf::Vector2i(820, 100));

    m_outsideBackground.setTexture(CTextureManager::instance().getTexture("outside_bg.png"));
    m_insideBackground.setTexture(CTextureManager::instance().getTexture("hive_bg.png"));

    // --- Génération des Fleurs (Champ du bas) ---
    float grassYStart = 500.f * (1.f - constants::GRASS_HEIGHT_RATIO);
    for(int i = 0; i < constants::INITIAL_FLOWER_COUNT; ++i) {
        float x = 50.f + (std::rand() % 600);
        float y = grassYStart + (std::rand() % (int)(500 * constants::GRASS_HEIGHT_RATIO - 50));
        m_flowers.push_back(std::make_unique<CFlower>(sf::Vector2f(x, y)));
    }

    // --- Population initiale ---
    m_entities.push_back(std::make_unique<CQueenBee>(sf::Vector2f(350.f, 250.f)));

    for(int i = 0; i < 10; ++i) {
        m_entities.push_back(std::make_unique<CWorkerBee>(
            sf::Vector2f(100.f + (rand() % 500), 100.f + (rand() % 300)),
            EWindowType::BEEHIVE
        ));
    }

    for(int i = 0; i < 3; ++i) {
        m_entities.push_back(std::make_unique<CWarriorBee>(
            sf::Vector2f(200.f + (rand() % 300), 200.f + (rand() % 200))
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
    // Update des fleurs
    for (auto& flower : m_flowers) {
        flower->update(dt);
    }

    // Update des abeilles
    for (auto& entity : m_entities) {
        EWindowType currentWin = entity->getWindowType();
        sf::Vector2u winSize = (currentWin == EWindowType::BEEHIVE) 
                               ? m_insideWindow.getSize() 
                               : m_outsideWindow.getSize();
        
        entity->update(dt, winSize);

        // Logique spécifique aux WorkerBee (Récolte et Transition)
        CWorkerBee* worker = dynamic_cast<CWorkerBee*>(entity.get());
        if (worker) {
            // Collision avec fleurs (si dehors et pas plein)
            if (currentWin == EWindowType::OUTSIDE && !worker->isFull()) {
                for (auto& flower : m_flowers) {
                    if (flower->isAvailable() && worker->getBounds().intersects(flower->getBounds())) {
                        worker->addPollen(flower->collect());
                        break; 
                    }
                }
            }

            // Transition Ruche -> Dehors (si vide, touche bord gauche)
            if (currentWin == EWindowType::BEEHIVE && !worker->isFull() && worker->getPosition().x <= 15) {
                worker->setWindowType(EWindowType::OUTSIDE);
                worker->setPosition(sf::Vector2f(winSize.x - 30, worker->getPosition().y));
            }
            // Transition Dehors -> Ruche (si plein, touche bord droit)
            else if (currentWin == EWindowType::OUTSIDE && worker->isFull() && worker->getPosition().x >= winSize.x - 15) {
                worker->setWindowType(EWindowType::BEEHIVE);
                worker->setPosition(sf::Vector2f(30, worker->getPosition().y));
                worker->resetPollen(); // Dépôt du pollen
            }
        }
    }
}

void CApplication::render()
{
    if (m_outsideWindow.isOpen()) {
        m_outsideWindow.clear();
        m_outsideWindow.draw(m_outsideBackground);
        for (auto& flower : m_flowers) flower->draw(m_outsideWindow);
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