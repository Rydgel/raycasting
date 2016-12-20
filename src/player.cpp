#include "player.h"
#include "utility/constants.h"


Player::Player(MapManager& map) : map_manager(map)
{
    this->x = 16.0;
    this->y = 10.0;
    this->dir = 0;
    this->rot = 0.0;
    this->speed = 0;
    this->sideSpeed = 0;
    this->move_speed = 4.0;
    this->rot_speed = 100.0 * Constants::PI / 180.0;
    this->map_manager = map;
}

void Player::update(const float dt)
{
    // Player will move this far along
    // the current direction vector
    double moveStep = this->speed * this->move_speed * dt;
    double moveSideStep = this->sideSpeed * this->move_speed * dt;

    // Add rotation if player is rotating (player.dir != 0)
    this->rot += this->dir * this->rot_speed * dt;

    // make sure the angle is between 0 and 360 degrees
    while (rot < 0) rot += 2 * Constants::PI;
    while (rot >= 2 * Constants::PI) rot -= 2 * Constants::PI;

    // Calculate new player position with simple trigonometry

    double newX = this->x + cos(this->rot) * moveStep - sin(this->rot) * moveSideStep;
    double newY = this->y + sin(this->rot) * moveStep + cos(this->rot) * moveSideStep;

    if (map_manager.isBlocking(newX, newY)) {   // are we allowed to move to the new position?
        return; // no, bail out.
    }

    // Set new position
    this->x = newX;
    this->y = newY;

    // At the end of the frame, the player dir and speed are reset
    this->dir = 0;
    this->rot_speed = 100.0 * Constants::PI / 180.0;
}
