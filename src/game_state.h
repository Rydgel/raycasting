#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "game.h"

class Game;

class GameState
{
public:

    Game* game;

    virtual void draw(const float dt) = 0;
    virtual void update(const float dt) = 0;
    virtual void handleInput(const float dt) = 0;
    virtual ~GameState() {};
};

#endif /* GAME_STATE_H */