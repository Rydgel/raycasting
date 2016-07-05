#ifndef OUTRUN_TEXTURE_MANAGER_H
#define OUTRUN_TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class TextureManager
{
private:

    /* Array of textures used */
    std::map<std::string, sf::Texture> textures;

public:

    /* Add a texture from a file */
    void loadTexture(const std::string& name, const std::string& filename, bool repeated = false, bool smooth = false);

    /* Translate an id into a reference */
    sf::Texture& getRef(const std::string& texture);

    /* Constructor */
    TextureManager()
    {
    }
};

#endif //OUTRUN_TEXTURE_MANAGER_H
