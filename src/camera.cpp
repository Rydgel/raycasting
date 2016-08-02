#include "camera.h"


Camera::Camera(Game *g, Player& p, MapManager& m)
: player(p)
, map(m)
{
    this->game = g;
    this->screen_width = game->screen_width;
    this->screen_height = game->screen_height;
    this->scale = game->scale;
    this->num_rays = (int) ceil(screen_width / strip_width);
    this->view_dist = (screen_width/2) / tan((fov / 2));
    initStripes();
}


void Camera::initStripes()
{
    for (int i = 0; i < screen_width; i += strip_width)
    {
        Stripe stripe(i, strip_width);
        stripes.push_back(stripe);
    }
}

void Camera::update()
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

void Camera::castSingleRay(const double rayAngle, const int stripIdx)
{
    // first make sure the angle is between 0 and 360 degrees
    double newAngle = fmod(rayAngle, 2 * M_PI);
    if (newAngle < 0) newAngle += 2 * M_PI;

    // moving right/left? up/down? Determined by which quadrant the angle is in.
    bool right = (newAngle > 2 * M_PI * 0.75 || newAngle < 2 * M_PI * 0.25);
    bool up = (newAngle < 0 || newAngle > M_PI);

    int wallType = 0;

    // only do these once
    double angleSin = sin(newAngle);
    double angleCos = cos(newAngle);

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
        dist = dist * cos(player.rot - newAngle);

        // now calc the position, height and width of the wall strip

        // "real" wall height in the game world is 1 unit, the distance from the player to the screen is viewDist,
        // thus the height on the screen is equal to wall_height_real * viewDist / dist
        double height = 1.5 * view_dist / dist;

        // top placement is easy since everything is centered on the x-axis, so we simply move
        // it half way down the screen and then half the wall height back up.
        double top = (screen_height - height) / 2;

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


void Camera::draw(sf::RenderWindow &w)
{
    for (const Stripe &stripe : stripes)
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
