#include "minimap.h"


Minimap::Minimap(Game *game, Player &p, MapManager &m, Camera& c)
: player(p)
, map(m)
, camera(c)
{
    this->game = game;
}

void Minimap::drawMinimap(sf::RenderWindow &w)
{
    for (int y = 0; y < this->map.mapHeight; ++y)
    {
        for (int x = 0; x < this->map.mapWidth; ++x)
        {
            int wall = this->map.worldMap[y][x];

            if (wall > 0)
            {
                sf::RectangleShape rectangle(sf::Vector2f(minimap_scale, minimap_scale));
                rectangle.setPosition(x * minimap_scale, y * minimap_scale);
                rectangle.setFillColor(sf::Color(200, 200, 200, 128));
                w.draw(rectangle);
            }
        }
    }

    this->drawPlayerMinimap(w);
}

void Minimap::drawPlayerMinimap(sf::RenderWindow &w)
{
    // player dot
    sf::RectangleShape rectangle(sf::Vector2f(4, 4));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));
    rectangle.setPosition((float) (player.x * minimap_scale - 2), (float) (player.y * minimap_scale - 2));
    w.draw(rectangle);
    // player direction line
    sf::Vector2f start(
            (float) (player.x * minimap_scale),
            (float) (player.y * minimap_scale));
    sf::Vector2f end(
            (float) ((player.x + cos(player.rot) * 4) * minimap_scale),
            (float) ((player.y + sin(player.rot) * 4) * minimap_scale));
    sf::Color color(255, 0, 0, 128);

    sf::VertexArray vertices(sf::Lines, 2);
    vertices[0] = sf::Vertex(start);
    vertices[1] = sf::Vertex(end);
    vertices[0].color = color;
    vertices[1].color = color;

    w.draw(vertices);
}

void Minimap::drawRay(sf::RenderWindow &w, double xHit, double yHit)
{
    sf::Vector2f start((float) (player.x * minimap_scale), (float) (player.y * minimap_scale));
    sf::Vector2f end((float) (xHit * minimap_scale), (float) (yHit * minimap_scale));
    sf::Color color(0, 100, 0, 255 / 3);

    sf::VertexArray vertices(sf::Lines, 2);
    vertices[0] = sf::Vertex(start);
    vertices[1] = sf::Vertex(end);
    vertices[0].color = color;
    vertices[1].color = color;

    w.draw(vertices);
}

void Minimap::drawRays(sf::RenderWindow &w)
{
    for (const Stripe &stripe : camera.stripes)
    {
        this->drawRay(w, stripe.xHit, stripe.yHit);
    }
}

void Minimap::draw(sf::RenderWindow &w)
{
    this->drawMinimap(w);
    this->drawRays(w);
}
