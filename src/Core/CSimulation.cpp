#include "Core/CSimulation.hpp"

// Constructeur de la simulation
CSimulation::CSimulation()
    : m_beehive()
{
    // Pour l'instant, aucune entité n'est ajoutée
    // La structure est prête pour évoluer
}

void CSimulation::update(float /*dt*/)
{
    // La logique de simulation sera ajoutée plus tard
    // Pour l'instant, cette méthode existe pour respecter l'architecture
}

void CSimulation::draw(sf::RenderWindow& /*hiveWindow*/,
                       sf::RenderWindow& /*outsideWindow*/)
{
    // Le rendu des entités sera implémenté plus tard
}
