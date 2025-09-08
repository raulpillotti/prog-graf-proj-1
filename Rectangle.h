#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"
#include "Primitives.h"
#include "Point.h" // Incluído para usar a classe Point
#include <SDL2/SDL.h>
#include <algorithm> // Para std::min/max

class Rectangle : public Shape {
private:
    // MUDANÇA: Armazena os cantos opostos como Pontos, contendo coords de mundo e tela
    Point p1;
    Point p2;
    Uint32 color;

public:
    // MUDANÇA: Construtor agora aceita coordenadas do mundo (floats)
    Rectangle(float world_x1, float world_y1, float world_x2, float world_y2, Uint32 color)
        : p1(world_x1, world_y1), p2(world_x2, world_y2), color(color) {}

    // NOVO: Método para normalizar as coordenadas do mundo para a tela
    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
        p1.normalize(metersX, metersY, screenWidth, screenHeight);
        p2.normalize(metersX, metersY, screenWidth, screenHeight);
    }

    // MUDANÇA: O método de desenho agora usa as coordenadas de tela (screen_)
    // e foi otimizado para desenhar linhas.
    void draw(SDL_Surface* surface) override {
        if (!surface) return;

        // Garante que desenhamos do canto superior esquerdo para o inferior direito
        int startX = std::min(p1.screen_x, p2.screen_x);
        int startY = std::min(p1.screen_y, p2.screen_y);
        int endX = std::max(p1.screen_x, p2.screen_x);
        int endY = std::max(p1.screen_y, p2.screen_y);

        // Desenha o retângulo preenchido usando linhas horizontais
        for (int y = startY; y < endY; y++) {
            Primitives::drawLine(surface, startX, y, endX, y, color);
        }
    }

    // MUDANÇA: Getters atualizados para retornar coordenadas de tela
    int getScreenX1() const { return p1.screen_x; }
    int getScreenY1() const { return p1.screen_y; }
    int getScreenX2() const { return p2.screen_x; }
    int getScreenY2() const { return p2.screen_y; }
};

#endif
