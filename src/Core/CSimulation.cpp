#include "Core/CSimulation.hpp"

// Au lancement, on est dehors
CSimulation::CSimulation()
: m_currentView(EWindowType::OUTSIDE)
{
}

EWindowType CSimulation::getCurrentView() const
{
    return m_currentView;
}

void CSimulation::switchView()
{
    if (m_currentView == EWindowType::OUTSIDE)
        m_currentView = EWindowType::BEEHIVE;
    else
        m_currentView = EWindowType::OUTSIDE;
}
