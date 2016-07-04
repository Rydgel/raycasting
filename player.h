#ifndef RAYCASTING_PLAYER_H
#define RAYCASTING_PLAYER_H

#import <cmath>

class Player
{
private:
    bool isBlocking(double x, double y);
public:
    double x;
    double y;
    int dir;
    double rot;
    int speed;
    double move_speed;
    double rot_speed;
    int worldMap[24][24];

    Player(int worldMap[24][24]);
    ~Player() { };
    void update();
};


#endif //RAYCASTING_PLAYER_H
