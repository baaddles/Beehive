#include "World/CScene.hpp"

CScene::CScene(EWindowType type)
    : m_type(type)
{
}

EWindowType CScene::getType() const
{
    return m_type;
}
