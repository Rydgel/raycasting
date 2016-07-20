#include <sstream>
#include <iostream>
#include "game_state_run.h"
#include "draw_utils.h"


void GameStateRun::drawRays(sf::RenderWindow& w)
{
    for (const Stripe &stripe : camera.stripes)
    {
        this->drawRay(w, stripe.xHit, stripe.yHit);
    }
}

void GameStateRun::drawRay(sf::RenderWindow& w, double xHit, double yHit)
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

void GameStateRun::drawMinimap(sf::RenderWindow& w, Player& player)
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

void GameStateRun::drawFloor(sf::RenderWindow &w)
{
    sf::Uint8* pixels = new sf::Uint8[game->screen_width * game->screen_height * 4];
    sf::Texture texture;
    texture.create(game->screen_width, game->screen_height);
    sf::Sprite sprite(texture);

    sf::Image& imageFloor = game->texmgr.getImageRef("wood_floor");
    sf::Image& imageCeil = game->texmgr.getImageRef("wood_ceil");

    // stuffs that don't change in the duration of a single frame
    double rCos = cos(player.rot);
    double rSin = sin(player.rot);
    int halfHeight = game->screen_height / 2;
    int halfWidth = game->screen_width / 2;

    for (int screenY = game->screen_height - 1; screenY >= halfHeight; --screenY) {
        double distance = game->screen_height / (2.0 * screenY - game->screen_height);
        distance = distance * 96;

        double horizontal_scale = 0.75 * distance / game->screen_height;

        double lineDX = -rSin * horizontal_scale;
        double lineDY = rCos * horizontal_scale;

        double spaceX = player.x * 48 + (distance * rCos) - halfWidth * lineDX;
        double spaceY = player.y * 48 + (distance * rSin) - halfWidth * lineDY;

        for (int screenX = 0; screenX <= game->screen_width - 1; ++screenX) {
            double texX = (int) floor(spaceX) & 63;
            double texY = (int) floor(spaceY) & 63;

            int floor = screenX + screenY * game->screen_width;
            int ceil = screenX + (game->screen_height - screenY) * game->screen_width;

            double shedding = 90 / distance;

            if (shedding < 0.0) {
                shedding = 0.0;
            }

            if (shedding > 1) {
                shedding = 1.0;
            }

            sf::Color cf = imageFloor.getPixel((unsigned int) texX, (unsigned int) texY);

            pixels[floor * 4] = (sf::Uint8) (cf.r * shedding);
            pixels[floor * 4 + 1] = (sf::Uint8) (cf.g * shedding);
            pixels[floor * 4 + 2] = (sf::Uint8) (cf.b * shedding);
            pixels[floor * 4 + 3] = cf.a;

            sf::Color cc = imageCeil.getPixel((unsigned int) texX, (unsigned int) texY);

            pixels[ceil * 4] = (sf::Uint8) (cc.r * shedding);
            pixels[ceil * 4 + 1] = (sf::Uint8) (cc.g * shedding);
            pixels[ceil * 4 + 2] = (sf::Uint8) (cc.b * shedding);
            pixels[ceil * 4 + 3] = cc.a;

            spaceX += lineDX;
            spaceY += lineDY;
        }
    }


    texture.update(pixels);
    sprite.setPosition(0, 0);
    sprite.scale(game->scale, game->scale);
    w.draw(sprite);
    delete[] pixels;
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

    drawFloor(this->game->window);
    camera.draw(this->game->window);

    // draw fps
    if (fps_counter)
    {
        drawFPSCounter(this->game->window, 1.0f / dt);
    }

    // draw minimap
    if (draw_minimap)
    {
        drawMinimap(this->game->window, player);
        drawRays(this->game->window);
    }

    return;
}

void GameStateRun::update(const float dt)
{
    player.update(dt);
    camera.update();
}

void GameStateRun::handleInput()
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

    return;
}

GameStateRun::GameStateRun(Game* game)
: player(map)
, camera(game, player, map)
{
    this->game = game;
    this->font.loadFromFile("resources/visitor1.ttf");
    int halfWidth = this->game->screen_width * this->game->scale / 2;
    int halfHeight = this->game->screen_height * this->game->scale / 2;
    sf::Mouse::setPosition(sf::Vector2i(halfWidth, halfHeight), this->game->window);
}

GameStateRun::~GameStateRun()
{

}
