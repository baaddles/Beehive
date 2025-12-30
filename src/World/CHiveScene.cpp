#include "World/CHiveScene.hpp"

CHiveScene::CHiveScene()
    : CScene(EWindowType::BEEHIVE)
{
}

void CHiveScene::handleEvent(
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

        sf::FloatRect actionZone = getActionZone(window.getSize());

        if (actionZone.contains(mousePos))
        {
            requestSceneSwitch = true;
        }
    }
}

void CHiveScene::update(float)
{
}

void CHiveScene::draw(sf::RenderWindow& window) const
{
    window.clear(sf::Color(200, 200, 200));

    sf::FloatRect zone = getActionZone(window.getSize());

    sf::RectangleShape exitRect;
    exitRect.setPosition(zone.left, zone.top);
    exitRect.setSize({ zone.width, zone.height });
    exitRect.setFillColor(sf::Color(139, 69, 19));

    window.draw(exitRect);
}

sf::FloatRect CHiveScene::getActionZone(
    const sf::Vector2u& windowSize
) const
{
    float width  = windowSize.x * 0.20f;
    float height = windowSize.y * 0.25f;

    return sf::FloatRect(
        windowSize.x * 0.75f,
        (windowSize.y - height) / 2.f,
        width,
        height
    );
}
