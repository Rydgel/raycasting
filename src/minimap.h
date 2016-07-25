#ifndef RAYCASTING_MINIMAP_H
#define RAYCASTING_MINIMAP_H


#include "game.h"
#include "player.h"
#include "camera.h"

class Minimap
{
public:
    Game *game;
    const Player& player;
    MapManager& map;
    Camera& camera;

    int minimap_scale = 16;

    Minimap(Game *game, Player& p, MapManager& m, Camera& c);
    void draw(sf::RenderWindow& w);
    ~Minimap() {};
private:
    void drawMinimap(sf::RenderWindow& w);
    void drawPlayerMinimap(sf::RenderWindow& w);
    void drawRays(sf::RenderWindow& w);
    void drawRay(sf::RenderWindow& w, double xHit, double yHit);
};


#endif //RAYCASTING_MINIMAP_H
