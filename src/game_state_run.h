#ifndef RAYCASTING_GAME_STATE_RUN_H
#define RAYCASTING_GAME_STATE_RUN_H


#include "game_state.h"
#include "player.h"
#include "map_manager.h"


class GameStateRun : public GameState
{
private:
    void drawMinimap(sf::RenderWindow& w, Player& player);
    void drawPlayerMinimap(sf::RenderWindow& w, Player& player);
public:
    Game *game;
    Player *player;
    MapManager map;

    int minimap_scale = 16;
    bool draw_minimap = true;

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    GameStateRun(Game *game);
    ~GameStateRun();
};


#endif //RAYCASTING_GAME_STATE_RUN_H
