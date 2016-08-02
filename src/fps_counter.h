#ifndef RAYCASTING_FPS_COUNTER_H
#define RAYCASTING_FPS_COUNTER_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <sstream>
#include <cmath>


class FpsCounter
{
private:

    sf::Font font;
    int total_frames = 0;
    float total_time = 0.0f;
    double fps = -1.0;

public:

    FpsCounter();
    void addFrameData(const float dt);
    void draw(sf::RenderWindow &w);
    ~FpsCounter() {};

};


#endif //RAYCASTING_FPS_COUNTER_H
