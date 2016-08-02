#ifndef RAYCASTING_GAME_STATE_RUN_H
#define RAYCASTING_GAME_STATE_RUN_H


#include "game_state.h"
#include "player.h"
#include "map_manager.h"
#include "stripe.h"
#include "camera.h"
#include "fps_counter.h"
#include "minimap.h"
#include "floor_ceiling.h"


class GameStateRun : public GameState
{
public:

    Game *game;
    Player player;
    MapManager map;
    Camera camera;
    FpsCounter fpsCounter;
    Minimap minimap;
    FloorCeiling floorCeiling;

    bool draw_minimap = false;
    bool fps_counter = true;

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput(const float dt);

    GameStateRun(Game *game);
    ~GameStateRun() {};
};


#endif //RAYCASTING_GAME_STATE_RUN_H
