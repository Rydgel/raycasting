#include <stack>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "game.h"
#include "game_state.h"

void Game::loadTextures()
{
    // texmgr.loadTexture("background", "images/bg.png", true, false);
}

void Game::pushState(GameState* state)
{
    this->states.push(state);

    return;
}

void Game::popState()
{
    delete this->states.top();
    this->states.pop();

    return;
}

void Game::changeState(GameState* state)
{
    if (!this->states.empty())
        popState();
    pushState(state);

    return;
}

GameState* Game::peekState()
{
    if(this->states.empty()) return nullptr;
    return this->states.top();
}

void Game::gameLoop()
{
    sf::Clock clock;

    while(this->window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        if(peekState() == nullptr) continue;
        peekState()->handleInput();
        peekState()->update(dt);
        this->window.clear(sf::Color(0, 0, 0));
        peekState()->draw(dt);
        this->window.display();
    }
}

Game::Game()
{
    this->loadTextures();

    this->window.create(sf::VideoMode(screen_width * scale, screen_height * scale), "raycasting!");
    // this->window.create(sf::VideoMode::getFullscreenModes()[0], "raycasting!", sf::Style::Fullscreen);
    // this->window.setFramerateLimit(60);
    // this->window.setVerticalSyncEnabled(true);
}

Game::~Game()
{
    while(!this->states.empty()) popState();
}