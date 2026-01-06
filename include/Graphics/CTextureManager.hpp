#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class CTextureManager
{
public:
    static CTextureManager& instance();

    const sf::Texture& getTexture(const std::string& name);

private:
    CTextureManager();
    void loadTexture(const std::string& name);

private:
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::string m_basePath = "assets/textures/";
};
