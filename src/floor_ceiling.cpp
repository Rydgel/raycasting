#include "floor_ceiling.h"


FloorCeiling::FloorCeiling(Game *game, Player &p, MapManager &m, Camera &c)
: player(p)
, map(m)
, camera(c)
{
    this->game = game;
    // init pixels buffer
    pixels = new sf::Uint8[game->screen_width * game->screen_height * 4];
    // init surface texture
    texture.create(game->screen_width, game->screen_height);
    sprite.setTexture(texture);
    sprite.setPosition(0, 0);
    sprite.scale(game->scale, game->scale);
}

void FloorCeiling::draw(sf::RenderWindow &w)
{
    // we can make this dynamic according to actual map later
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
    w.draw(sprite);
}

FloorCeiling::~FloorCeiling()
{
    delete[] pixels;
}

