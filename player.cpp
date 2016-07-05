#include "player.h"


Player::Player(MapManager *map)
{
    x = 16.0;
    y = 10.0;
    dir = 0;
    rot = 0.0;
    speed = 0;
    move_speed = 0.18;
    rot_speed = 6.0 * M_PI / 180.0;
    this->map = map;
}

void Player::update()
{
    // Player will move this far along
    // the current direction vector
    double moveStep = speed * move_speed;

    // Add rotation if player is rotating (player.dir != 0)
    rot += dir * rot_speed;

    // Calculate new player position with simple trigonometry
    double newX = x + cos(rot) * moveStep;
    double newY = y + sin(rot) * moveStep;

    if (map->isBlocking(newX, newY)) {   // are we allowed to move to the new position?
        return; // no, bail out.
    }

    // Set new position
    x = newX;
    y = newY;
}
