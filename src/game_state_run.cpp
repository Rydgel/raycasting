#include <sstream>
#include <iostream>
#include "game_state_run.h"


void GameStateRun::initStripes()
{
    for (int i = 0; i < game->screen_width; i += strip_width)
    {
        Stripe stripe(i, strip_width);
        stripes.push_back(stripe);
    }
}

void GameStateRun::castRays()
{
    int stripIdx = 0;

    for (int i = 0; i < num_rays; ++i)
    {
        // where on the screen does ray go through?
        double rayScreenPos = (-num_rays/2 + i) * strip_width;
        // the distance from the viewer to the point on the screen, simply Pythagoras.
        double rayViewDist = sqrt(rayScreenPos * rayScreenPos + view_dist * view_dist);
        // the angle of the ray, relative to the viewing direction.
        // right triangle: a = sin(A) * c
        double rayAngle = asin(rayScreenPos / rayViewDist);
        this->castSingleRay(player.rot + rayAngle, ++stripIdx);
    }
}

void GameStateRun::castSingleRay(double rayAngle, int stripIdx)
{
    // first make sure the angle is between 0 and 360 degrees
    rayAngle = fmod(rayAngle, 2 * M_PI);
    if (rayAngle < 0) rayAngle += 2 * M_PI;

    // moving right/left? up/down? Determined by which quadrant the angle is in.
    bool right = (rayAngle > 2 * M_PI * 0.75 || rayAngle < 2 * M_PI * 0.25);
    bool up = (rayAngle < 0 || rayAngle > M_PI);

    int wallType = 0;

    // only do these once
    double angleSin = sin(rayAngle);
    double angleCos = cos(rayAngle);

    double dist = 0.0;   // the distance to the block we hit
    double xHit = 0;        // the x and y coord of where the ray hit the block
    double yHit = 0;

    int wallX;      // the (x,y) map coords of the block
    int wallY;

    bool wallIsHorizontal = false;

    // first check against the vertical map/wall lines
    // we do this by moving to the right or left edge of the block we're standing in
    // and then moving in 1 map unit steps horizontally. The amount we have to move vertically
    // is determined by the slope of the ray, which is simply defined as sin(angle) / cos(angle).

    double slope = angleSin / angleCos;     // the slope of the straight line made by the ray
    int dXVer = right ? 1 : -1;             // we move either 1 map unit to the left or right
    double dYVer = dXVer * slope;           // how much to move up or down

    double x = right ? ceil(player.x) : floor(player.x);  // starting horizontal position, at one of the edges of the current map block
    double y = player.y + (x - player.x) * slope;         // starting vertical position. We add the small horizontal step we just made, multiplied by the slope.

    while (x >= 0 && x < map.mapWidth && y >= 0 && y < map.mapHeight)
    {
        wallX = (int) floor(x + (right ? 0 : -1));
        wallY = (int) floor(y);

        // is this point inside a wall block?
        if (map.worldMap[wallY][wallX] > 0)
        {
            double distX = x - player.x;
            double distY = y - player.y;
            dist = distX*distX + distY*distY;      // the distance from the player to this point, squared.

            wallType = map.worldMap[wallY][wallX]; // we'll remember the type of wall we hit for later

            xHit = x;   // save the coordinates of the hit. We only really use these to draw the rays on minimap.
            yHit = y;

            wallIsHorizontal = false;

            break;
        }
        x += dXVer;
        y += dYVer;
    }

    // now check against horizontal lines. It's basically the same, just "turned around".
    // the only difference here is that once we hit a map block,
    // we check if there we also found one in the earlier, vertical run. We'll know that if dist != 0.
    // If so, we only register this hit if this distance is smaller.

    slope = angleCos / angleSin;
    double dYHor = up ? -1 : 1;
    double dXHor = dYHor * slope;
    y = up ? floor(player.y) : ceil(player.y);
    x = player.x + (y - player.y) * slope;

    while (x >= 0 && x < map.mapWidth && y >= 0 && y < map.mapHeight)
    {
        wallY = (int) floor(y + (up ? -1 : 0));
        wallX = (int) floor(x);
        if (map.worldMap[wallY][wallX] > 0)
        {
            double distX = x - player.x;
            double distY = y - player.y;
            double blockDist = distX*distX + distY*distY;
            if (!dist || blockDist < dist) {
                dist = blockDist;
                xHit = x;
                yHit = y;

                wallType = map.worldMap[wallY][wallX];

                wallIsHorizontal = true;
            }

            break;
        }
        x += dXHor;
        y += dYHor;
    }

    if (dist)
    {
        // we update the stripe
        Stripe stripe = stripes[stripIdx - 1];

        dist = sqrt(dist);

        // use perpendicular distance to adjust for fish eye
        // distorted_dist = correct_dist / cos(relative_angle_of_ray)
        dist = dist * cos(player.rot - rayAngle);

        // now calc the position, height and width of the wall strip

        // "real" wall height in the game world is 1 unit, the distance from the player to the screen is viewDist,
        // thus the height on the screen is equal to wall_height_real * viewDist / dist
        double height = 1.5 * view_dist / dist;

        // top placement is easy since everything is centered on the x-axis, so we simply move
        // it half way down the screen and then half the wall height back up.
        double top = (game->screen_height - height) / 2;

        stripe.height = height;
        stripe.top = top;
        stripe.width = strip_width;
        stripe.xHit = xHit;
        stripe.yHit = yHit;
        stripe.dist = dist;
        stripe.wallIsHorizontal = wallIsHorizontal;
        stripe.wallType = wallType;

        stripes[stripIdx - 1] = stripe;
    }
}

void GameStateRun::drawRays(sf::RenderWindow& w)
{
    for (Stripe &stripe : stripes)
    {
        this->drawRay(w, stripe.xHit, stripe.yHit);
    }
}

void GameStateRun::drawRay(sf::RenderWindow& w, double xHit, double yHit)
{
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f((float) (player.x * minimap_scale), (float) (player.y * minimap_scale))),
                    sf::Vertex(sf::Vector2f(
                            (float) (xHit * minimap_scale),
                            (float) (yHit * minimap_scale))
                    )
            };

    line[0].color = sf::Color(0, 100, 0, 255 / 3);
    line[1].color = sf::Color(0, 100, 0, 255 / 3);
    w.draw(line, 2, sf::Lines);
}

void GameStateRun::drawMinimap(sf::RenderWindow& w, Player& player)
{
    for (int y = 0; y < this->map.mapHeight; ++y)
    {
        for (int x = 0; x < this->map.mapWidth; ++x)
        {
            int wall = this->map.worldMap[y][x];

            if (wall > 0)
            {
                sf::RectangleShape rectangle(sf::Vector2f(minimap_scale, minimap_scale));
                rectangle.setPosition(x * minimap_scale, y * minimap_scale);
                rectangle.setFillColor(sf::Color(200, 200, 200, 128));
                w.draw(rectangle);
            }
        }
    }

    this->drawPlayerMinimap(w, player);
}

void GameStateRun::drawPlayerMinimap(sf::RenderWindow& w, Player& player)
{
    // player dot
    sf::RectangleShape rectangle(sf::Vector2f(4, 4));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));
    rectangle.setPosition((float) (player.x * minimap_scale - 2), (float) (player.y * minimap_scale - 2));
    w.draw(rectangle);
    // player direction line
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f((float) (player.x * minimap_scale), (float) (player.y * minimap_scale))),
                    sf::Vertex(sf::Vector2f(
                            (float) ((player.x + cos(player.rot) * 4) * minimap_scale),
                            (float) ((player.y + sin(player.rot) * 4) * minimap_scale))
                    )
            };

    line[0].color = sf::Color(255, 0, 0, 128);
    line[1].color = sf::Color(255, 0, 0, 128);
    w.draw(line, 2, sf::Lines);
}

void GameStateRun::drawCamera(sf::RenderWindow &w)
{
    int scale = game->scale;
    for (Stripe &stripe : stripes)
    {
        sf::Texture& tex = game->texmgr.getRef(map.getTextureName(stripe.wallType));

        int offset;

        if (stripe.wallIsHorizontal)
            offset = (int) fmod(stripe.xHit * 64, 64);
        else
            offset = (int) fmod(stripe.yHit * 64, 64);

        float left = (float) (stripe.left * scale);
        float top = (float) (stripe.top * scale);
        float width = (float) stripe.width * scale;
        float height = (float) stripe.height * scale;
        double dist = stripe.dist;
        double shedding;

        if (round(dist) == 0) {
            shedding = 1;
        } else {
            shedding = 2 / dist;
        }

        if (shedding > 1) {
            shedding = 1;
        }

        if (shedding < 0) {
            shedding = 0;
        }

        sf::VertexArray slice(sf::Quads, 4);
        slice[0].position = sf::Vector2f(left, top);
        slice[1].position = sf::Vector2f(left + width, top);
        slice[2].position = sf::Vector2f(left + width, top + height);
        slice[3].position = sf::Vector2f(left, top + height);

        slice[0].texCoords = sf::Vector2f(offset, 0);
        slice[1].texCoords = sf::Vector2f(offset + strip_width, 0);
        slice[2].texCoords = sf::Vector2f(offset + strip_width, 96);
        slice[3].texCoords = sf::Vector2f(offset, 96);

        for (int i = 0; i < 4; ++i)
        {
            slice[i].color.r = (sf::Uint8) (slice[i].color.r * shedding);
            slice[i].color.g = (sf::Uint8) (slice[i].color.g * shedding);
            slice[i].color.b = (sf::Uint8) (slice[i].color.b * shedding);
        }

        w.draw(slice, &tex);
    }
}

void GameStateRun::drawFloor(sf::RenderWindow &w)
{
    sf::Uint8* pixels = new sf::Uint8[game->screen_width * game->screen_height * 4];
    sf::Texture texture;
    texture.create(game->screen_width, game->screen_height);
    sf::Sprite sprite(texture);

    sf::Image& imageFloor = game->texmgr.getImageRef("wood_floor");
    sf::Image& imageCeil = game->texmgr.getImageRef("wood_ceil");

    // stuffs that don't change in the duration of a single frame
    double rCos = cos(player.rot);
    double rSin = sin(player.rot);
    int halfHeight = game->screen_height / 2;
    int halfWidth = game->screen_width / 2;

    for (int screenY = game->screen_height - 1; screenY >= halfHeight; --screenY) {
        double distance = game->screen_height / (2.0 * screenY - game->screen_height);
        distance = distance * 96;

        double horizontal_scale = 0.75 * distance / game->screen_height;

        double lineDX = -rSin * horizontal_scale;
        double lineDY = rCos * horizontal_scale;

        double spaceX = player.x * 48 + (distance * rCos) - halfWidth * lineDX;
        double spaceY = player.y * 48 + (distance * rSin) - halfWidth * lineDY;

        for (int screenX = 0; screenX <= game->screen_width - 1; ++screenX) {
            double texX = (int) floor(spaceX) & 63;
            double texY = (int) floor(spaceY) & 63;

            int floor = screenX + screenY * game->screen_width;
            int ceil = screenX + (game->screen_height - screenY) * game->screen_width;

            double shedding = 90 / distance;

            if (shedding < 0.0) {
                shedding = 0.0;
            }

            if (shedding > 1) {
                shedding = 1.0;
            }

            sf::Color cf = imageFloor.getPixel((unsigned int) texX, (unsigned int) texY);

            pixels[floor * 4] = (sf::Uint8) (cf.r * shedding);
            pixels[floor * 4 + 1] = (sf::Uint8) (cf.g * shedding);
            pixels[floor * 4 + 2] = (sf::Uint8) (cf.b * shedding);
            pixels[floor * 4 + 3] = cf.a;

            sf::Color cc = imageCeil.getPixel((unsigned int) texX, (unsigned int) texY);

            pixels[ceil * 4] = (sf::Uint8) (cc.r * shedding);
            pixels[ceil * 4 + 1] = (sf::Uint8) (cc.g * shedding);
            pixels[ceil * 4 + 2] = (sf::Uint8) (cc.b * shedding);
            pixels[ceil * 4 + 3] = cc.a;

            spaceX += lineDX;
            spaceY += lineDY;
        }
    }


    texture.update(pixels);
    sprite.setPosition(0, 0);
    sprite.scale(game->scale, game->scale);
    w.draw(sprite);
}

void GameStateRun::drawFPSCounter(sf::RenderWindow& w, float fps)
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


void GameStateRun::draw(const float dt)
{
    this->game->window.clear(sf::Color::Black);
    // this->game->window.draw(this->background);

    drawFloor(this->game->window);
    // drawCeiling(this->game->window);
    drawCamera(this->game->window);

    // draw fps
    if (fps_counter) {
        drawFPSCounter(this->game->window, 1.0f / dt);
    }

    // draw minimap
    if (draw_minimap)
    {
        drawMinimap(this->game->window, player);
        drawRays(this->game->window);
    }

    return;
}

void GameStateRun::update(const float dt)
{
    player.update(dt);
    castRays();
}

void GameStateRun::handleInput()
{
    sf::Event event;

    while(this->game->window.pollEvent(event))
    {
        switch (event.type)
        {
            // we don't process other types of events
            default:
                break;
            // window closed
            case sf::Event::Closed:
                this->game->window.close();
                break;
                // key pressed
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Z:
                        player.speed = 1;
                        break;
                    case sf::Keyboard::S:
                        player.speed = -1;
                        break;
                    case sf::Keyboard::Q:
                        player.sideSpeed = -1;
                        break;
                    case sf::Keyboard::D:
                        player.sideSpeed = 1;
                        break;
                    case sf::Keyboard::M:
                        draw_minimap = !draw_minimap; break;
                    case sf::Keyboard::F:
                        fps_counter = !fps_counter; break;
                    case sf::Keyboard::Escape:
                        this->game->window.close(); break;
                    default: break;
                }
                break;
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                    case sf::Keyboard::Z:
                    case sf::Keyboard::S:
                        player.speed = 0; break;
                    case sf::Keyboard::Q:
                    case sf::Keyboard::D:
                        player.sideSpeed = 0; break;
                    default: break;
                }
                break;
            case sf::Event::MouseMoved:
                int current_x = sf::Mouse::getPosition(this->game->window).x;
                int halfWidth = this->game->screen_width * this->game->scale / 2;
                int halfHeight = this->game->screen_height * this->game->scale / 2;
                int elapsed_x = halfWidth - current_x;

                if (elapsed_x != 0) {
                    player.dir = -elapsed_x / abs(elapsed_x);
                    player.rot_speed = abs(elapsed_x) * M_PI / 10;
                    sf::Mouse::setPosition(sf::Vector2i(halfWidth, halfHeight), this->game->window);
                } else {
                    player.dir = 0;
                    player.rot_speed = 100.0 * M_PI / 180.0;
                }
                break;
        }
    }

    return;
}

GameStateRun::GameStateRun(Game* game) : player(map)
{
    this->game = game;
    this->font.loadFromFile("resources/visitor1.ttf");
    // camera settings
    num_rays = (int) ceil(game->screen_width / strip_width);
    view_dist = (game->screen_width/2) / tan((fov / 2));
    int halfWidth = this->game->screen_width * this->game->scale / 2;
    int halfHeight = this->game->screen_height * this->game->scale / 2;
    sf::Mouse::setPosition(sf::Vector2i(halfWidth, halfHeight), this->game->window);
    // stripes init
    this->initStripes();
}

GameStateRun::~GameStateRun()
{

}
