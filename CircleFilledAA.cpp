#include "CircleFilledAA.h"
#include "Primitives.h"
#include <cmath>
#include <algorithm> 

CircleFilledAA::CircleFilledAA(Point center, float radius, Uint32 color)
    : center(center), world_radius(radius), color(color), screen_radius(0) {}

void CircleFilledAA::normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
    center.normalize(metersX, metersY, screenWidth, screenHeight);

    if (metersX > 0 && metersY > 0) {
        float scaleX = screenWidth / metersX;
        float scaleY = screenHeight / metersY;
        screen_radius = static_cast<int>(world_radius * (scaleX + scaleY) / 2.0f);
    }
}

void CircleFilledAA::draw(SDL_Surface* surface) {
    if (!surface || screen_radius <= 0) return;

    Uint8 r, g, b;
    SDL_GetRGB(color, surface->format, &r, &g, &b);
    int xc = center.screen_x;
    int yc = center.screen_y;

    for (int y = -screen_radius - 1; y <= screen_radius + 1; y++) {
        for (int x = -screen_radius - 1; x <= screen_radius + 1; x++) {
            float dist = std::sqrt(float(x*x + y*y));
            float coverage = (float)screen_radius + 0.5f - dist;

            if (coverage > 0) {
                coverage = std::min(coverage, 1.0f);
                Uint32 pixelColor = Primitives::getPixel(surface, xc + x, yc + y);
                Uint8 pr, pg, pb;
                SDL_GetRGB(pixelColor, surface->format, &pr, &pg, &pb);

                Uint8 nr = static_cast<Uint8>(pr * (1 - coverage) + r * coverage);
                Uint8 ng = static_cast<Uint8>(pg * (1 - coverage) + g * coverage);
                Uint8 nb = static_cast<Uint8>(pb * (1 - coverage) + b * coverage);

                Uint32 newColor = SDL_MapRGB(surface->format, nr, ng, nb);


                Primitives::setPixel(surface, xc + x, yc + y, newColor);
            }
        }
    }
}

