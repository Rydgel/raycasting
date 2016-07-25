#ifndef RAYCASTING_GAME_STATE_RUN_H
#define RAYCASTING_GAME_STATE_RUN_H


#include "game_state.h"
#include "player.h"
#include "map_manager.h"
#include "stripe.h"
#include "camera.h"
#include "fps_counter.h"
#include "minimap.h"


class GameStateRun : public GameState
{
public:
    Game *game;
    Player player;
    MapManager map;
    Camera camera;
    FpsCounter fpsCounter;
    Minimap minimap;

    bool draw_minimap = false;
    bool fps_counter = true;

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    GameStateRun(Game *game);
    ~GameStateRun();
private:
    void drawFloor(sf::RenderWindow& w);
};


#endif //RAYCASTING_GAME_STATE_RUN_H
