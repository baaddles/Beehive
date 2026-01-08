#pragma once
#include "World/CEntity.hpp"
#include "Utils/CEnums.hpp"

class CBee : public CEntity
{
protected:
    float m_speed;
    sf::Vector2f m_homePosition; 
    sf::Sprite m_sprite; 

    void keepInsideWindow(const sf::Vector2u& windowSize);

public:
    CBee(sf::Vector2f pos, EWindowType winType, float speed);
    virtual ~CBee() = default;

    // Overrides de CEntity
    virtual void update(float dt, const sf::Vector2u& windowSize) override;
    virtual void draw(sf::RenderWindow& window) const override;
    virtual sf::FloatRect getBounds() const override;
    
    // Stats génériques pour le Hover
    virtual std::string getStats() const override { return "Abeille"; }

    // Méthode spécifique aux abeilles (Level Up)
    virtual void levelUpStats(float factor) = 0;

    // Getters / Setters spécifiques
    void setHomePosition(sf::Vector2f pos) { m_homePosition = pos; }
    sf::Vector2f getHomePosition() const { return m_homePosition; }
    float getSpeed() const { return m_speed; }
};