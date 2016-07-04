#include <cstring>
#include <cstdio>
#include "player.h"


Player::Player(int map[24][24])
{
    x = 16.0;
    y = 10.0;
    dir = 0;
    rot = 0.0;
    speed = 0;
    move_speed = 0.18;
    rot_speed = 6.0 * M_PI / 180.0;
    memcpy(worldMap, map, 24 * 24 * sizeof(**map));

    for (int y = 0; y < 24; ++y)
    {
        for (int x = 0; x < 24; ++x)
        {
            printf("%d", worldMap[y][x]);
        }
        printf("\n");
    }
}

bool Player::isBlocking(double x, double y)
{
    // First make sure that we cannot move
    // outside the boundaries of the level
    if (y < 0 || y >= 24 || x < 0 || x >= 24) {
        return true;
    }
    // Return true if the map block is not 0,
    // i.e. if there is a blocking wall.
    return (worldMap[(int) floor(y)][(int) floor(x)] != 0);
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

    if (isBlocking(newX, newY)) {   // are we allowed to move to the new position?
        return; // no, bail out.
    }

    // Set new position
    x = newX;
    y = newY;
}
