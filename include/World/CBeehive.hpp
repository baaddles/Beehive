#pragma once

class CBeehive
{
private:
    int m_bees; // nombre d'abeilles
    int m_nectar; // quantité de nectar collecté ramené dans la ruche
    int m_beesMax; // nombre max d'abeilles pouvant vivre dans la ruche
    int m_nectarMax; // quantité max de nectar -> si atteint, produit une abeille (que si m_bees < m_beesMax) et reinitialise m_nectar à 0

public:
    CBeehive();

    void update(float dt);
};