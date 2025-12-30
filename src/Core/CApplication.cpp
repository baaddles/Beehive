#include "Core/CApplication.hpp"

CApplication::CApplication()
    : m_window(sf::VideoMode(1000, 700), "Beehive"),
      m_currentScene(std::make_unique<COutsideScene>())
{
    m_window.setFramerateLimit(60);
}

void CApplication::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        handleEvents();
        update(dt);
        render();
    }
}

void CApplication::handleResize(const sf::Event& event)
{
    sf::FloatRect visibleArea(
        0.f, 0.f,
        static_cast<float>(event.size.width),
        static_cast<float>(event.size.height)
    );
    m_window.setView(sf::View(visibleArea));
}

void CApplication::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if (event.type == sf::Event::Resized){
            handleResize(event);
        }

        bool requestSceneSwitch = false;
        m_currentScene->handleEvent(event, m_window, requestSceneSwitch);

        if (requestSceneSwitch){
            switchScene();
            requestSceneSwitch = false;
        }
    }
}

void CApplication::update(float dt)
{
    m_currentScene->update(dt);
}

void CApplication::render()
{
    m_window.clear();
    m_currentScene->draw(m_window);
    m_window.display();
}


void CApplication::switchScene()
{
    if (m_currentScene->getType() == EWindowType::OUTSIDE)
        m_currentScene = std::make_unique<CHiveScene>();
    else
        m_currentScene = std::make_unique<COutsideScene>();
}
