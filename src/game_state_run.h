#ifndef RAYCASTING_GAME_STATE_RUN_H
#define RAYCASTING_GAME_STATE_RUN_H


#include "game_state.h"
#include "player.h"
#include "map_manager.h"
#include "stripe.h"


class GameStateRun : public GameState
{
public:
    Game *game;
    Player player;
    MapManager map;

    // camera stuff
    int strip_width = 1;
    double fov = M_PI * 60 / 180;
    int num_rays;
    double view_dist;

    // stripes drawings
    std::vector<Stripe> stripes;

    // assets
    sf::Font font;

    int minimap_scale = 16;
    bool draw_minimap = false;
    bool fps_counter = true;

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    GameStateRun(Game *game);
    ~GameStateRun();
private:
    void drawMinimap(sf::RenderWindow& w, Player& player);
    void drawPlayerMinimap(sf::RenderWindow& w, Player& player);
    void drawFPSCounter(sf::RenderWindow& w, float fps);
    void drawRays(sf::RenderWindow& w);
    void drawRay(sf::RenderWindow& w, double xHit, double yHit);
    void drawCamera(sf::RenderWindow& w);
    void drawFloor(sf::RenderWindow& w);
    // camera
    void castRays();
    void castSingleRay(double rayAngle, int stripIdx);
    // stripes
    void initStripes();
};


#endif //RAYCASTING_GAME_STATE_RUN_H
