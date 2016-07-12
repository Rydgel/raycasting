#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#include "texture_manager.h"


void TextureManager::loadTexture(const std::string& name, const std::string& filename, bool repeated, bool smooth)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);
    tex.setRepeated(repeated);
    tex.setSmooth(smooth);

    /* Add it to the list of textures */
    this->textures[name] = tex;
    this->images[name] = tex.copyToImage();

    return;
}

sf::Texture& TextureManager::getRef(const std::string& texture)
{
    return this->textures.at(texture);
}

sf::Image& TextureManager::getImageRef(const std::string& texture)
{
    return this->images.at(texture);
}