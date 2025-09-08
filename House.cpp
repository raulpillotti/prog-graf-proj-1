#include "House.h"
#include "Point.h"
#include <vector>

House::House() {}

void House::build(float x, float y, float width, float height, Uint32 wallColor, Uint32 roofColor, Uint32 doorColor) {
    wall.setPoints({
        Point(x, y),
        Point(x + width, y),
        Point(x + width, y + height),
        Point(x, y + height)
    });
    wall.setColor(wallColor);

    float roofHeight = height * 0.5f;
    float roofOverhang = width * 0.1f;
    roof.setPoints({
        Point(x - roofOverhang, y + height),
        Point(x + width + roofOverhang, y + height),
        Point(x + width / 2.0f, y + height + roofHeight)
    });
    roof.setColor(roofColor);

    float doorWidth = width * 0.25f;
    float doorHeight = height * 0.6f;
    float doorX = x + (width - doorWidth) / 2.0f;
    door.setPoints({
        Point(doorX, y),
        Point(doorX + doorWidth, y),
        Point(doorX + doorWidth, y + doorHeight),
        Point(doorX, y + doorHeight)
    });
    door.setColor(doorColor);
}

void House::normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
    wall.normalize(metersX, metersY, screenWidth, screenHeight);
    roof.normalize(metersX, metersY, screenWidth, screenHeight);
    door.normalize(metersX, metersY, screenWidth, screenHeight);
}

void House::draw(SDL_Surface* surface) {
    wall.draw(surface);
    roof.draw(surface);
    door.draw(surface);
}