#include "Sun.h"
#include "Point.h"

Sun::Sun() : body(Point(0,0), 0, 0) {}

void Sun::build(float x, float y, float width, float height, Uint32 color) {
    float radius = (width + height) / 4.0f;
    float centerX = x + width / 2.0f;
    float centerY = y + height / 2.0f;
    body = CircleFilledAA(Point(centerX, centerY), radius, color);
}

void Sun::normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
    body.normalize(metersX, metersY, screenWidth, screenHeight);
}

void Sun::draw(SDL_Surface* surface) {
    body.draw(surface);
}
