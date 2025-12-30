// CSimulation.hpp
#pragma once
#include "Utils/CEnums.hpp"

class CSimulation
{
private:
    EWindowType m_currentView; // vue active (OUTSIDE ou HIVE)

public:
    CSimulation();

    EWindowType getCurrentView() const;
    void switchView();
};
