#pragma once

enum class EWindowType // pour constamment savoir dans quelle fenêtre une entité se situe
{
    BEEHIVE,
    OUTSIDE
};

enum class EWorkerState // état d'une abeille ouvrière
{
    MORT,
    DANS_RUCHE,
    TRAVAILLE
};

enum class EWarriorState // état d'une abeille guerrière
{
    MORT,
    REPOS,
    DEFENSE,
    SOIN
};