#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"
#include "Point.h"
#include "matrix.h"
#include <vector>
#include <SDL2/SDL.h>

class Polygon : public Shape {
private:
    std::vector<Point> worldPoints;
    std::vector<Point> normalizedPoints;
    Uint32 color;

public:
    Polygon() : color(0xFFFFFFFF) {}

    // Define os pontos no sistema de coordenadas do mundo
    void setPoints(const std::vector<Point>& newPoints);

    // Define a cor de preenchimento
    void setColor(Uint32 newColor);

    // NEW: Aplica uma matriz de transformação (ex: rotação) aos pontos do mundo
    void transform(const Matrix& m);

    // MODIFIED: Converte os worldPoints em normalizedPoints sem destruir os originais
    void normalize(float metersX, float metersY, int screenWidth, int screenHeight);

    // MODIFIED: O desenho agora preenche o polígono e será implementado no .cpp
    void draw(SDL_Surface* surface) override;
};

#endif