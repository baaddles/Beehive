#include "World/COutsideScene.hpp"
#include "World/CBees/CWorkerBee.hpp"

COutsideScene::COutsideScene()
    : CScene(EWindowType::OUTSIDE)
{
    m_entities.push_back(
        std::make_unique<CWorkerBee>(sf::Vector2f(500.f, 350.f))
    );    
}

void COutsideScene::handleEvent(
    const sf::Event& event,
    const sf::RenderWindow& window,
    bool& requestSceneSwitch
)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(
            static_cast<float>(event.mouseButton.x),
            static_cast<float>(event.mouseButton.y)
        );

        sf::Vector2u size = window.getSize();
        sf::FloatRect actionZone = getActionZone(size);

        if (actionZone.contains(mousePos))
        {
            requestSceneSwitch = true;
        }
    }
}

void COutsideScene::update(float dt)
{
    for (auto& entity : m_entities)
    {
        entity->update(dt);
    }
}

void COutsideScene::draw(sf::RenderWindow& window) const
{
    window.clear(sf::Color(100, 180, 255));

    sf::FloatRect zone = getActionZone(window.getSize());
    sf::RectangleShape hiveRect;
    hiveRect.setPosition(zone.left, zone.top);
    hiveRect.setSize({ zone.width, zone.height });
    hiveRect.setFillColor(sf::Color(255, 165, 0));
    window.draw(hiveRect);

    for (const auto& entity : m_entities)
    {
        entity->draw(window);
    }
}


sf::FloatRect COutsideScene::getActionZone(
    const sf::Vector2u& windowSize
) const
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

