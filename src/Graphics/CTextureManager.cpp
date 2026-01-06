#include "Graphics/CTextureManager.hpp"
#include <iostream>

#ifndef ASSETS_PATH
    #define ASSETS_PATH "assets/textures/"
#endif

CTextureManager& CTextureManager::instance()
{
    static CTextureManager instance;
    return instance;
}

CTextureManager::CTextureManager()
{
    m_basePath = ASSETS_PATH;
    std::cout << "[DEBUG] Chemin des assets configure : " << m_basePath << std::endl;
    
    // On charge default.png d'abord
    loadTexture("default.png");
}

void CTextureManager::loadTexture(const std::string& name)
{
    sf::Texture texture;
    std::string fullPath = m_basePath + name;
    
    if (!texture.loadFromFile(fullPath))
    {
        std::cerr << "[ERREUR] Impossible de charger : " << fullPath << std::endl;
        // Si c'est le default.png qui foire, on crée une texture colorée de secours
        if (name == "default.png") {
            sf::Image img;
            img.create(32, 32, sf::Color::Magenta);
            texture.loadFromImage(img);
            m_textures[name] = texture;
        }
        return;
    }
    
    std::cout << "[SUCCESS] Texture chargee : " << fullPath << std::endl;
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