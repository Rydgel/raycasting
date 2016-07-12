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

std::string MapManager::getTextureName(int wall_type)
{
    switch (wall_type)
    {
        case 1:
            return "wall_1";
        case 2:
            return "wall_2";
        case 3:
            return "wall_3";
        case 4:
            return "wall_4";
        default:
            return "";
    }
}

