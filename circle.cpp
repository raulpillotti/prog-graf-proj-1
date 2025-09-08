#include "Circle.h"

Circle::Circle(Point center, float radius, Uint32 color)
    : center(center), world_radius(radius), color(color), screen_radius(0) {}

void Circle::normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
    center.normalize(metersX, metersY, screenWidth, screenHeight);


    if (metersX > 0 && metersY > 0) {
        float scaleX = screenWidth / metersX;
        float scaleY = screenHeight / metersY;
        screen_radius = static_cast<int>(world_radius * (scaleX + scaleY) / 2.0f);
    }
}

void Circle::draw(SDL_Surface* surface) {
    if (!surface || screen_radius <= 0) return;

    int xc = center.screen_x;
    int yc = center.screen_y;

    int x = 0;
    int y = screen_radius;
    int d = 3 - 2 * screen_radius;

    auto drawCirclePoints = [&](int cx, int cy, int px, int py) {
        Primitives::setPixel(surface, cx + px, cy + py, color);
        Primitives::setPixel(surface, cx - px, cy + py, color);
        Primitives::setPixel(surface, cx + px, cy - py, color);
        Primitives::setPixel(surface, cx - px, cy - py, color);
        Primitives::setPixel(surface, cx + py, cy + px, color);
        Primitives::setPixel(surface, cx - py, cy + px, color);
        Primitives::setPixel(surface, cx + py, cy - px, color);
        Primitives::setPixel(surface, cx - py, cy - px, color);
    };

    drawCirclePoints(xc, yc, x, y);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        drawCirclePoints(xc, yc, x, y);
    }
}
