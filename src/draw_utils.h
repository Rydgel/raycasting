#ifndef RAYCASTING_DRAW_UTILS_H
#define RAYCASTING_DRAW_UTILS_H


#include <SFML/System.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>

class DrawUtils
{
public:
    static sf::VertexArray line(sf::Vector2f start, sf::Vector2f end, sf::Color color);
};


#endif //RAYCASTING_DRAW_UTILS_H
