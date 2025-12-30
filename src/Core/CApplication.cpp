#include "Core/CApplication.hpp"

CApplication::CApplication()
: m_window(sf::VideoMode(800, 600), "Beehive")
{
    m_window.setFramerateLimit(60); // 60 FPS fixes
}

void CApplication::run()
{
    while (m_window.isOpen())
    {
        handleEvents();
        render();
    }
}

sf::FloatRect getHiveZone(const sf::Vector2u& windowSize)
{
    float width  = windowSize.x * 0.25f;
    float height = windowSize.y * 0.30f;

    return sf::FloatRect(
        windowSize.x * 0.05f,
        (windowSize.y - height) / 2.f,
        width,
        height
    );
}

sf::FloatRect getExitZone(const sf::Vector2u& windowSize)
{
    float width  = windowSize.x * 0.20f;
    float height = windowSize.y * 0.20f;

    return sf::FloatRect(
        windowSize.x * 0.75f,
        (windowSize.y - height) / 2.f,
        width,
        height
    );
}

void CApplication::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(
                static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y)
            );

            // Zone cliquable (même position pour simplifier)
            sf::Vector2u size = m_window.getSize();

            sf::FloatRect actionZone =
                (m_simulation.getCurrentView() == EWindowType::OUTSIDE)
                ? getHiveZone(size)
                : getExitZone(size);

            if (actionZone.contains(mousePos))
            {
                m_simulation.switchView();
            }

        }

        if (event.type == sf::Event::Resized)
        {
            sf::FloatRect visibleArea(
                0.f,
                0.f,
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height)
            );

            m_window.setView(sf::View(visibleArea));
        }
    }
}

void CApplication::render()
{
    const sf::Vector2u windowSize = m_window.getSize();

    // Fond selon la vue
    if (m_simulation.getCurrentView() == EWindowType::OUTSIDE)
        m_window.clear(sf::Color(100, 180, 255)); // ciel
    else
        m_window.clear(sf::Color(218, 159, 90)); // intérieur ruche

    // Sélection de la zone active
    sf::FloatRect zone =
        (m_simulation.getCurrentView() == EWindowType::OUTSIDE)
        ? getHiveZone(windowSize)
        : getExitZone(windowSize);

    // Création du rectangle graphique à partir de la zone logique
    sf::RectangleShape actionRect;
    actionRect.setPosition(zone.left, zone.top);
    actionRect.setSize({ zone.width, zone.height });

    // Couleur selon la vue
    if (m_simulation.getCurrentView() == EWindowType::OUTSIDE)
        actionRect.setFillColor(sf::Color(255, 165, 0)); // orange (ruche)
    else
        actionRect.setFillColor(sf::Color(139, 69, 19)); // marron (sortie)

    m_window.draw(actionRect);
    m_window.display();
}

