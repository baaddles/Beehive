#include "Graphics/CTextureManager.hpp"
#include <iostream>

CTextureManager& CTextureManager::instance()
{
    static CTextureManager instance;
    return instance;
}

CTextureManager::CTextureManager()
{
    loadTexture("default.png");
}

void CTextureManager::loadTexture(const std::string& name)
{
    sf::Texture texture;
    if (!texture.loadFromFile(m_basePath + name))
    {
        std::cerr << "Erreur chargement texture : " << name << std::endl;
        return;
    }
    m_textures[name] = texture;
}

const sf::Texture& CTextureManager::getTexture(const std::string& name)
{
    auto it = m_textures.find(name);
    if (it != m_textures.end())
        return it->second;

    // tentative de chargement
    loadTexture(name);

    it = m_textures.find(name);
    if (it != m_textures.end())
        return it->second;

    // fallback sécurisé
    auto def = m_textures.find("default.png");
    if (def != m_textures.end())
        return def->second;

    // dernier recours ABSOLU (ne devrait jamais arriver)
    static sf::Texture empty;
    return empty;
}

