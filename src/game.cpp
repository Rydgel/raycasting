#include <iostream>
#include "game.h"


void Game::loadTextures()
{
    texmgr.loadTexture("barrel", "resources/tex/barrel.png", false, false);
    texmgr.loadTexture("bluestone", "resources/tex/bluestone.png", false, false);
    texmgr.loadTexture("colorstone", "resources/tex/colorstone.png", false, false);
    texmgr.loadTexture("eagle", "resources/tex/eagle.png", false, false);
    texmgr.loadTexture("greenlight", "resources/tex/greenlight.png", false, false);
    texmgr.loadTexture("greystone", "resources/tex/greystone.png", false, false);
    texmgr.loadTexture("mossy", "resources/tex/mossy.png", false, false);
    texmgr.loadTexture("pillar", "resources/tex/pillar.png", false, false);
    texmgr.loadTexture("purplestone", "resources/tex/purplestone.png", false, false);
    texmgr.loadTexture("redbrick", "resources/tex/redbrick.png", false, false);
    texmgr.loadTexture("wood", "resources/tex/wood.png", false, false);

    texmgr.loadTexture("wood_floor", "resources/tex/wood_floor.png", false, false);
    texmgr.loadTexture("wood_ceil", "resources/tex/wood_ceil.png", false, false);
    texmgr.loadTexture("wall_1", "resources/tex/wall_1.png", false, false);
    texmgr.loadTexture("wall_2", "resources/tex/wall_2.png", false, false);
    texmgr.loadTexture("wall_3", "resources/tex/wall_3.png", false, false);
    texmgr.loadTexture("wall_4", "resources/tex/wall_4.png", false, false);
    texmgr.loadTexture("door", "resources/tex/door.png", false, false);
}

void Game::pushState(GameState* state)
{
    this->states.push(state);
}

void Game::popState()
{
    delete this->states.top();
    this->states.pop();
}

void Game::changeState(GameState* state)
{
    if (!this->states.empty())
        popState();
    pushState(state);
}

GameState* Game::peekState()
{
    if (this->states.empty()) return nullptr;
    return this->states.top();
}

void Game::gameLoop()
{
    sf::Clock clock;

    while (this->window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        if (peekState() == nullptr) continue;
        peekState()->handleInput(dt);
        peekState()->update(dt);
        this->window.clear(sf::Color(0, 0, 0));
        peekState()->draw(dt);
        this->window.display();
    }
}

Game::Game()
{
    this->loadTextures();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    // this->window.create(sf::VideoMode(screen_width * scale, screen_height * scale), "", sf::Style::None, settings);
    // unsigned int width = sf::VideoMode::getFullscreenModes()[0].width;
    // unsigned int height = sf::VideoMode::getFullscreenModes()[0].height;
    this->window.create(sf::VideoMode::getFullscreenModes()[0], "raycasting!", sf::Style::Fullscreen, settings);
    sf::View view;
    view = this->window.getView();
    view.setSize(screen_width * scale, screen_height * scale);
    view.setCenter(screen_width * scale / 2 , screen_height * scale / 2);
    this->window.setView(view);
    // this->window.setView(sf::View());
    // this->window.setFramerateLimit(60);
    this->window.setVerticalSyncEnabled(true);
    this->window.setMouseCursorVisible(false);
    this->window.setActive(true);
    // this->window.setPosition(sf::Vector2i(0, 0));
    // this->window.setSize(sf::Vector2u(width, height));
}

Game::~Game()
{
    while (!this->states.empty()) popState();
}