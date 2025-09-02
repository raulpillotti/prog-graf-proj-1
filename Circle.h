#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL2/SDL.h>
#include "Shape.h"
#include "Primitives.h"

/*
Explicação:
-----------
xc, yc → centro do círculo.

radius → raio.

drawCirclePoints → função lambda para desenhar os 8 pontos simétricos (círculo tem simetria de 8 octantes).

O loop principal implementa o algoritmo do ponto médio de Bresenham, garantindo desempenho e precisão sem usar floats.

A função Primitives::setPixel é usada para realmente desenhar o pixel.
*/

class Circle : public Shape {
private:
    int xc, yc;      // centro
    int radius;      // raio
    Uint32 color;

public:
    Circle(int xc, int yc, int radius, Uint32 color)
        : xc(xc), yc(yc), radius(radius), color(color) {}

    void draw(SDL_Surface* surface) override {
        if (!surface) return;

        int x = 0;
        int y = radius;
        int d = 1 - radius;

        // desenha os pontos simétricos
        auto drawCirclePoints = [&](int cx, int cy, int x, int y) {
            Primitives::setPixel(surface, cx + x, cy + y, color);
            Primitives::setPixel(surface, cx - x, cy + y, color);
            Primitives::setPixel(surface, cx + x, cy - y, color);
            Primitives::setPixel(surface, cx - x, cy - y, color);
            Primitives::setPixel(surface, cx + y, cy + x, color);
            Primitives::setPixel(surface, cx - y, cy + x, color);
            Primitives::setPixel(surface, cx + y, cy - x, color);
            Primitives::setPixel(surface, cx - y, cy - x, color);
        };

        drawCirclePoints(xc, yc, x, y);

        while (x < y) {
            x++;
            if (d < 0) {
                d += 2 * x + 1;
            } else {
                y--;
                d += 2 * (x - y) + 1;
            }
            drawCirclePoints(xc, yc, x, y);
        }
    }
};

#endif
