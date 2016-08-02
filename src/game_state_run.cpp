#include "game_state_run.h"


void GameStateRun::draw(const float dt)
{
    this->game->window.clear(sf::Color::Black);

    floorCeiling.draw(this->game->window);
    camera.draw(this->game->window);
    // draw minimap
    if (draw_minimap)
        minimap.draw(this->game->window);
    // draw fps
    if (fps_counter)
        fpsCounter.draw(this->game->window);
}

void GameStateRun::update(const float dt)
{
    player.update(dt);
    camera.update();
    fpsCounter.addFrameData(dt);
}

void GameStateRun::handleInput(const float dt)
{
    sf::Event event;

    while(this->game->window.pollEvent(event))
    {
        switch (event.type)
        {
            // we don't process other types of events
            default:
                break;
            // window closed
            case sf::Event::Closed:
                this->game->window.close();
                break;
                // key pressed
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Z:
                        player.speed = 1;
                        break;
                    case sf::Keyboard::S:
                        player.speed = -1;
                        break;
                    case sf::Keyboard::Q:
                        player.sideSpeed = -1;
                        break;
                    case sf::Keyboard::D:
                        player.sideSpeed = 1;
                        break;
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
                switch (event.key.code)
                {
                    case sf::Keyboard::Z:
                    case sf::Keyboard::S:
                        player.speed = 0; break;
                    case sf::Keyboard::Q:
                    case sf::Keyboard::D:
                        player.sideSpeed = 0; break;
                    default: break;
                }
                break;
            case sf::Event::MouseMoved:
                int current_x = sf::Mouse::getPosition(this->game->window).x;
                int halfWidth = this->game->screen_width * this->game->scale / 2;
                int halfHeight = this->game->screen_height * this->game->scale / 2;
                int elapsed_x = halfWidth - current_x;

                if (elapsed_x != 0) {
                    player.dir = -elapsed_x / abs(elapsed_x);
                    player.rot_speed = abs(elapsed_x) * M_PI / 10;
                    sf::Mouse::setPosition(sf::Vector2i(halfWidth, halfHeight), this->game->window);
                } else {
                    player.dir = 0;
                    player.rot_speed = 100.0 * M_PI / 180.0;
                }
                break;
        }
    }
}

GameStateRun::GameStateRun(Game* game)
: player(map)
, camera(game, player, map)
, fpsCounter()
, minimap(game, player, map, camera)
, floorCeiling(game, player, map, camera)
{
    this->game = game;
    int halfWidth = this->game->screen_width * this->game->scale / 2;
    int halfHeight = this->game->screen_height * this->game->scale / 2;
    sf::Mouse::setPosition(sf::Vector2i(halfWidth, halfHeight), this->game->window);
}
