#include <iostream>
#include "game_state_run.h"

using namespace std;


int main(int argc, char **argv)
{
    Game game;
    game.pushState(new GameStateRun(&game));
    game.gameLoop();

    return EXIT_SUCCESS;
}