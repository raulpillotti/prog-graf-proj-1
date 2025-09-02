#ifndef CIRCLE_FILLED_H
#define CIRCLE_FILLED_H

#include <SDL2/SDL.h>
#include <cmath>
#include "Shape.h"
#include "Primitives.h"

class CircleFilled : public Shape {
private:
    int xc, yc;      // centro
    int radius;      // raio
    Uint32 color;

public:
    CircleFilled(int xc, int yc, int radius, Uint32 color)
        : xc(xc), yc(yc), radius(radius), color(color) {}

    void draw(SDL_Surface* surface) override {
        if (!surface) return;

        for (int y = -radius; y <= radius; y++) {
            int dx = (int)std::sqrt(radius * radius - y * y);  // metade da largura da linha
            Primitives::drawLine(surface, xc - dx, yc + y, xc + dx, yc + y, color);
        }
    }
};

#endif
