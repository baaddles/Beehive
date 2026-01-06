#include "Graphics/CTextureManager.hpp"
#include <iostream>

CTextureManager& CTextureManager::instance()
{
    static CTextureManager instance;
    return instance;
}

CTextureManager::CTextureManager()
{
    // On s'assure d'avoir une texture par défaut
    loadTexture("default.png");
}

void CTextureManager::loadTexture(const std::string& name)
{
    sf::Texture texture;
    std::string fullPath = m_basePath + name;
    
    if (!texture.loadFromFile(fullPath))
    {
        std::cerr << "Erreur : Impossible de trouver la texture : " << fullPath << std::endl;
        std::cerr << "Verifiez que le dossier assets est bien a la racine du projet." << std::endl;
        return;
    }
    m_textures[name] = texture;
}

const sf::Texture& CTextureManager::getTexture(const std::string& name)
{
    auto it = m_textures.find(name);
    if (it != m_textures.end())
        return it->second;

    loadTexture(name);

    it = m_textures.find(name);
    if (it != m_textures.end())
        return it->second;

    return m_textures["default.png"];
}