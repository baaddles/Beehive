#include "Core/CApplication.hpp"

// Constructeur de l'application
CApplication::CApplication()
    // Initialisation de la fenêtre "Beehive"
    : m_hiveWindow(
        sf::VideoMode(600, 400),      // Taille de la fenêtre
        "Beehive"                     // Titre
      ),
      // Initialisation de la fenêtre "Outside"
      m_outsideWindow(
        sf::VideoMode(800, 400),
        "Outside"
      )
{
    // Limitation du framerate à 60 FPS pour chaque fenêtre
    // Cela garantit une animation fluide et une charge CPU raisonnable
    m_hiveWindow.setFramerateLimit(60);
    m_outsideWindow.setFramerateLimit(60);
}

// Méthode principale de l'application
void CApplication::run()
{
    // Horloge SFML permettant de mesurer le temps entre deux frames
    sf::Clock clock;

    // Boucle principale : tant que les deux fenêtres sont ouvertes
    while (m_hiveWindow.isOpen() && m_outsideWindow.isOpen())
    {
        // Calcul du temps écoulé depuis la dernière frame
        // (utile plus tard pour la simulation)
        float dt = clock.restart().asSeconds();

        sf::Event event;

        // === Gestion des événements de la fenêtre ruche ===
        while (m_hiveWindow.pollEvent(event))
        {
            // Si l'utilisateur ferme la fenêtre
            if (event.type == sf::Event::Closed)
                m_hiveWindow.close();
        }

        // === Gestion des événements de la fenêtre extérieur ===
        while (m_outsideWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_outsideWindow.close();
        }

        // === Mise à jour de la simulation ===
        // Pour l'instant, la simulation est vide
        // mais l'appel est déjà en place (architecture évolutive)
        m_simulation.update(dt);

        // === Effacement des fenêtres ===
        // Couleurs choisies pour différencier visuellement les zones
        m_hiveWindow.clear(sf::Color(220, 180, 80));     // couleur miel
        m_outsideWindow.clear(sf::Color(37, 150, 190)); // couleur ciel

        // === Dessin de la simulation ===
        m_simulation.draw(m_hiveWindow, m_outsideWindow);

        // === Affichage final ===
        m_hiveWindow.display();
        m_outsideWindow.display();
    }
}
