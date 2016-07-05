#include "player.h"


Player::Player(MapManager *map)
{
    this->x = 16.0;
    this->y = 10.0;
    this->dir = 0;
    this->rot = 0.0;
    this->speed = 0;
    this->move_speed = 0.18;
    this->rot_speed = 6.0 * M_PI / 180.0;
    this->map = map;
}

void Player::update()
{
    // Player will move this far along
    // the current direction vector
    double moveStep = this->speed * this->move_speed;

    // Add rotation if player is rotating (player.dir != 0)
    this->rot += this->dir * this->rot_speed;

    // Calculate new player position with simple trigonometry
    double newX = this->x + cos(this->rot) * moveStep;
    double newY = this->y + sin(this->rot) * moveStep;

    if (this->map->isBlocking(newX, newY)) {   // are we allowed to move to the new position?
        return; // no, bail out.
    }

    // Set new position
    this->x = newX;
    this->y = newY;
}