#ifndef GAME_H
#define GAME_H

#include <stack>
#include <SFML/Graphics.hpp>
#include "texture_manager.h"
#include "game_state.h"

class GameState;

class Game
{
private:

    void loadTextures();

public:

    unsigned int screen_width = 320;
    unsigned int screen_height = 200;
    unsigned int scale = 8;

    std::stack<GameState*> states;

    sf::RenderWindow window;
    TextureManager texmgr;

    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* peekState();

    void gameLoop();

    Game();
    ~Game();
};

#endif /* GAME_H */