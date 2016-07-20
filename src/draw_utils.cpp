#include <SFML/Graphics/VertexArray.hpp>
#include "draw_utils.h"


sf::VertexArray DrawUtils::line(sf::Vector2f start, sf::Vector2f end, sf::Color color)
{
    sf::VertexArray vertices(sf::Lines, 2);
    vertices[0] = sf::Vertex(start);
    vertices[1] = sf::Vertex(end);
    vertices[0].color = color;
    vertices[1].color = color;

    return vertices;
}
