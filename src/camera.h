#ifndef RAYCASTING_CAMERA_H
#define RAYCASTING_CAMERA_H

#include <math.h>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include "stripe.h"
#include "player.h"
#include "game.h"


class Camera
{
public:
    Game *game;
    const Player& player;
    MapManager& map;
    // camera stuff
    int screen_width;
    int screen_height;
    int scale;
    int strip_width = 1;
    double fov = M_PI * 60 / 180;
    int num_rays;
    double view_dist;
    // stripes drawings
    std::vector<Stripe> stripes;

    Camera(Game *g, Player& p, MapManager& m);
    void update();
    void draw(sf::RenderWindow &w);
    ~Camera() {};
private:
    // stripes
    void initStripes();
    void castSingleRay(double rayAngle, int stripIdx);
};


#endif //RAYCASTING_CAMERA_H
