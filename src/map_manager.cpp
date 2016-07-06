#include <cmath>
#include "map_manager.h"


bool MapManager::isBlocking(double x, double y)
{
    // First make sure that we cannot move
    // outside the boundaries of the level
    if (y < 0 || y >= mapHeight || x < 0 || x >= mapWidth) {
        return true;
    }
    // Return true if the map block is not 0,
    // i.e. if there is a blocking wall.
    return (worldMap[(int) floor(y)][(int) floor(x)] != 0);
}