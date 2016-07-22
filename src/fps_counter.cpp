#include "fps_counter.h"


FpsCounter::FpsCounter()
{
    font.loadFromFile("resources/visitor1.ttf");
}

void FpsCounter::addFrameData(float dt)
{
    total_frames ++;
    total_time += dt;

    if (total_time >= 1.0)
    {
        fps = total_frames / total_time;
        total_frames = 0;
        total_time = 0.0;
    }
}

void FpsCounter::draw(sf::RenderWindow &w)
{
    if (fps != -1.0)
    {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(40);
        text.setStyle(sf::Text::Bold);
        text.setColor(sf::Color::White);
        text.setPosition(0, 0);

        std::ostringstream ss;
        ss << round(fps);
        text.setString(ss.str());
        w.draw(text);
    }
}
