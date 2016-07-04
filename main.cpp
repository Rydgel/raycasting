#include <iostream>
#include <SFML/Graphics.hpp>
#include "player.h"

using namespace std;


#define mapWidth 24
#define mapHeight 24

int minimap_scale = 16;

int worldMap[mapWidth][mapHeight] =
        {
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
                {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
                {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };

void drawMinimap(sf::RenderWindow& w, Player& player)
{
    for (int y = 0; y < mapHeight; ++y)
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            int wall = worldMap[y][x];


            if (wall > 0) {
                sf::RectangleShape rectangle(sf::Vector2f(minimap_scale, minimap_scale));
                rectangle.setPosition(x * minimap_scale, y * minimap_scale);
                rectangle.setFillColor(sf::Color(200, 200, 200));
                w.draw(rectangle);
            }
        }
    }

    // draw player
    // player dot
    sf::RectangleShape rectangle(sf::Vector2f(4, 4));
    rectangle.setFillColor(sf::Color(255, 0, 0));
    rectangle.setPosition((float) (player.x * minimap_scale - 2), (float) (player.y * minimap_scale - 2));
    w.draw(rectangle);
    // player direction line
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f((float) (player.x * minimap_scale), (float) (player.y * minimap_scale))),
                    sf::Vertex(sf::Vector2f(
                            (float) ((player.x + cos(player.rot) * 4) * minimap_scale),
                            (float) ((player.y + sin(player.rot) * 4) * minimap_scale))
                    )
            };

    line[0].color = sf::Color(255, 0, 0);
    line[1].color = sf::Color(255, 0, 0);
    w.draw(line, 2, sf::Lines);
}

int main(int argc, char **argv)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "raycasting");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Player player = Player::Player(worldMap);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    window.close();
                    break;
                    // key pressed
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                            player.speed = 1; break;
                        case sf::Keyboard::Down:
                            player.speed = -1; break;
                        case sf::Keyboard::Left:
                            player.dir = -1; break;
                        case sf::Keyboard::Right:
                            player.dir = 1; break;
                        default: break;
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                        case sf::Keyboard::Down:
                            player.speed = 0; break;
                        case sf::Keyboard::Left:
                        case sf::Keyboard::Right:
                            player.dir = 0; break;
                        default: break;
                    }
                    break;
                    // we don't process other types of events
                default:
                    break;
            }
        }

        // move player
        player.update();

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw minimap
        drawMinimap(window, player);

        // end the current frame
        window.display();
    }

    return EXIT_SUCCESS;
}