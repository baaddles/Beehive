#include "Core/CApplication.hpp"
#include "Utils/CConstants.hpp"

CApplication::CApplication()
    : m_outsideWindow(sf::VideoMode(700, 500), "Outside - Nature"),
      m_insideWindow(sf::VideoMode(700, 500), "Inside - Beehive")
{
    m_outsideWindow.setFramerateLimit(60);
    m_insideWindow.setFramerateLimit(60);

    m_insideWindow.setPosition(sf::Vector2i(100, 100));
    m_outsideWindow.setPosition(sf::Vector2i(820, 100));

    m_outsideBackground.setTexture(CTextureManager::instance().getTexture("outside_bg.png"));
    m_insideBackground.setTexture(CTextureManager::instance().getTexture("hive_bg.png"));

    float grassHeight = 500.f * constants::GRASS_HEIGHT_RATIO;
    m_grassRect.setSize(sf::Vector2f(700.f, grassHeight));
    m_grassRect.setFillColor(constants::GRASS_COLOR);
    m_grassRect.setPosition(0.f, 500.f - grassHeight);

    for(int i = 0; i < constants::INITIAL_FLOWER_COUNT; ++i) {
        float x = 50.f + (std::rand() % 600);
        float y = (500.f - grassHeight) + (std::rand() % (int)(grassHeight - 40));
        m_flowers.push_back(std::make_unique<CFlower>(sf::Vector2f(x, y)));
    }

    m_entities.push_back(std::make_unique<CQueenBee>(sf::Vector2f(350.f, 250.f)));

    for(int i = 0; i < 10; ++i) {
        m_entities.push_back(std::make_unique<CWorkerBee>(
            sf::Vector2f(100.f + (rand() % 500), 100.f + (rand() % 300)),
            EWindowType::BEEHIVE
        ));
    }
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
    while (m_outsideWindow.pollEvent(event)) if (event.type == sf::Event::Closed) m_outsideWindow.close();
    while (m_insideWindow.pollEvent(event)) if (event.type == sf::Event::Closed) m_insideWindow.close();
}

void CApplication::update(float dt) {
    // 1. Compter les fleurs disponibles
    int availableFlowers = 0;
    for (auto& flower : m_flowers) {
        flower->update(dt);
        if (flower->isAvailable()) availableFlowers++;
    }

    for (auto& entity : m_entities) {
        EWindowType currentWin = entity->getWindowType();
        sf::Vector2u winSize = (currentWin == EWindowType::BEEHIVE) ? m_insideWindow.getSize() : m_outsideWindow.getSize();
        
        entity->update(dt, winSize);

        CWorkerBee* worker = dynamic_cast<CWorkerBee*>(entity.get());
        if (worker) {
            // DEHORS : Recherche de fleurs
            if (currentWin == EWindowType::OUTSIDE && !worker->isFull() && worker->getBehavior() == EWorkerBehavior::WANDERING) {
                worker->searchFlower(m_flowers);
            }

            // TRANSITION RUCHE -> DEHORS 
            // Condition supplémentaire : S'il y a des fleurs disponibles
            if (currentWin == EWindowType::BEEHIVE && !worker->isFull() && worker->getBehavior() == EWorkerBehavior::WANDERING && availableFlowers > 0) {
                sf::Vector2f pos = worker->getPosition();
                worker->setPosition(sf::Vector2f(pos.x + 150.f * dt, pos.y));
                if (pos.x >= (float)winSize.x - 5.f) {
                    worker->setWindowType(EWindowType::OUTSIDE);
                    worker->setPosition(sf::Vector2f(10.f, pos.y));
                }
            }
            // TRANSITION DEHORS -> RUCHE
            else if (currentWin == EWindowType::OUTSIDE && (worker->isFull() || worker->getBehavior() == EWorkerBehavior::RETURNING)) {
                sf::Vector2f pos = worker->getPosition();
                if (pos.x <= 5.f) {
                    worker->setWindowType(EWindowType::BEEHIVE);
                    worker->setPosition(sf::Vector2f((float)winSize.x - 15.f, pos.y));
                    worker->startDelivering();
                }
            }
        }
    }
}

void CApplication::render() {
    if (m_outsideWindow.isOpen()) {
        m_outsideWindow.clear();
        m_outsideWindow.draw(m_outsideBackground);
        m_outsideWindow.draw(m_grassRect);
        for (auto& flower : m_flowers) flower->draw(m_outsideWindow);
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