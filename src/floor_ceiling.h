#ifndef RAYCASTING_FLOOR_CEILING_H
#define RAYCASTING_FLOOR_CEILING_H


#include "game.h"
#include "map_manager.h"
#include "player.h"
#include "camera.h"

class FloorCeiling
{
public:
    Game *game;
    const Player& player;
    MapManager& map;
    Camera& camera;

    FloorCeiling(Game *game, Player &p, MapManager &m, Camera& c);
    void draw(sf::RenderWindow& w);
    ~FloorCeiling();
private:
    // pixels buffer
    sf::Uint8* pixels;
    // texture buffer
    sf::Texture texture;
    sf::Sprite sprite;
};


#endif //RAYCASTING_FLOOR_CEILING_H
