#ifndef TREE_H
#define TREE_H

#include "Polygon.h"
#include "Shape.h"
#include <vector>

class Tree : public Shape {
private:
    Polygon trunk;  // Tronco da árvore
    Polygon leaves; // Folhas/Copa da árvore

public:
    // Construtor padrão
    Tree();

    // NOVO: Método para construir a árvore com base nos parâmetros do CSV
    void build(float x, float y, float width, float height, Uint32 trunkColor, Uint32 leavesColor, float inclination);

    // MUDANÇA: Implementa o método normalize, conforme exigido por Shape
    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) override;

    // O método draw chama o draw das partes internas
    void draw(SDL_Surface* surface) override;
};

#endif