#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL2/SDL.h>
#include "Shape.h"
#include "Primitives.h"

class Rectangle : public Shape {
private:
    int x1, y1, x2, y2;
    Uint32 color;

public:
    Rectangle(int x1, int y1, int x2, int y2, Uint32 color)
        : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {}

    void draw(SDL_Surface* surface) override {
        if (!surface) return;

        // Desenhar retângulo preenchido usando Primitives
        for (int y = y1; y < y2; y++) {
            for (int x = x1; x < x2; x++) {
                Primitives::setPixel(surface, x, y, color);
            }
        }

        // Alternativamente, poderia usar apenas as arestas:
        // Primitives::drawLine(surface, x1, y1, x2, y1, color); // topo
        // Primitives::drawLine(surface, x1, y2, x2, y2, color); // base
        // Primitives::drawLine(surface, x1, y1, x1, y2, color); // esquerda
        // Primitives::drawLine(surface, x2, y1, x2, y2, color); // direita
    }

    int getX1() {
        return x1;
    }

    int getY1() {
        return y1;
    }

    int getX2() {
        return x2;
    }

    int getY2() {
        return y2;
    }

};

#endif

