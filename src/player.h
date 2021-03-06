#ifndef RAYCASTING_PLAYER_H
#define RAYCASTING_PLAYER_H

#include <cmath>
#include "map_manager.h"

class Player
{
public:

    double x;
    double y;
    int dir;
    double rot;
    int speed;
    int sideSpeed;
    double move_speed;
    double rot_speed;

    MapManager& map_manager;

    Player(MapManager& map);
    ~Player() { };
    void update(const float dt);
};


#endif //RAYCASTING_PLAYER_H
