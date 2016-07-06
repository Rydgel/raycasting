#include <sstream>
#include "game_state_run.h"


void GameStateRun::drawMinimap(sf::RenderWindow& w, Player& player)
{
    for (int y = 0; y < this->map.mapHeight; ++y)
    {
        for (int x = 0; x < this->map.mapWidth; ++x)
        {
            int wall = this->map.worldMap[y][x];

            if (wall > 0) {
                sf::RectangleShape rectangle(sf::Vector2f(minimap_scale, minimap_scale));
                rectangle.setPosition(x * minimap_scale, y * minimap_scale);
                rectangle.setFillColor(sf::Color(200, 200, 200, 128));
                w.draw(rectangle);
            }
        }
    }

    this->drawPlayerMinimap(w, player);
}

void GameStateRun::drawPlayerMinimap(sf::RenderWindow& w, Player& player)
{
    // player dot
    sf::RectangleShape rectangle(sf::Vector2f(4, 4));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));
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

    line[0].color = sf::Color(255, 0, 0, 128);
    line[1].color = sf::Color(255, 0, 0, 128);
    w.draw(line, 2, sf::Lines);
}

void GameStateRun::drawFPSCounter(sf::RenderWindow& w, float fps)
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(40);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::White);
    text.setPosition(0, 0);

    std::ostringstream ss;
    ss << round(fps);

    text.setString(ss.str());

    w.draw(text);
}


void GameStateRun::draw(const float dt)
{
    this->game->window.clear(sf::Color::Black);
    // this->game->window.draw(this->background);

    // draw fps
    if (fps_counter) {
        drawFPSCounter(this->game->window, 1.0f / dt);
    }

    // draw minimap
    if (draw_minimap) {
        drawMinimap(this->game->window, *player);
    }

    return;
}

void GameStateRun::update(const float dt)
{
    player->update(dt);

}

void GameStateRun::handleInput()
{
    sf::Event event;

    while(this->game->window.pollEvent(event))
    {
        switch (event.type)
        {
            // window closed
            case sf::Event::Closed:
                this->game->window.close();
                break;
                // key pressed
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        player->speed = 1; break;
                    case sf::Keyboard::Down:
                        player->speed = -1; break;
                    case sf::Keyboard::Left:
                        player->dir = -1; break;
                    case sf::Keyboard::Right:
                        player->dir = 1; break;
                    case sf::Keyboard::M:
                        draw_minimap = !draw_minimap; break;
                    case sf::Keyboard::F:
                        fps_counter = !fps_counter; break;
                    case sf::Keyboard::Escape:
                        this->game->window.close(); break;
                    default: break;
                }
                break;
            case sf::Event::KeyReleased:
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                    case sf::Keyboard::Down:
                        player->speed = 0; break;
                    case sf::Keyboard::Left:
                    case sf::Keyboard::Right:
                        player->dir = 0; break;
                    default: break;
                }
                break;
                // we don't process other types of events
            default:
                break;
        }
    }

    return;
}

GameStateRun::GameStateRun(Game* game)
{
    this->game = game;
    this->player = new Player(&map);
    this->font.loadFromFile("resources/visitor1.ttf");
}

GameStateRun::~GameStateRun()
{
    delete player;
}

