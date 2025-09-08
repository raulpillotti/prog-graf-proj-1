#ifndef SUN_H
#define SUN_H

#include "Shape.h"
#include "CircleFilledAA.h" // MUDANÇA: Incluído para usar a primitiva de círculo
#include "Point.h"

class Sun : public Shape {
private:
    // MUDANÇA: O corpo do sol agora é um círculo com anti-aliasing para suavidade
    CircleFilledAA body;

public:
    // Construtor padrão
    Sun();

    // NOVO: Método para construir o sol a partir dos dados do CSV
    void build(float x, float y, float width, float height, Uint32 color);

    // MUDANÇA: Agora implementa o método normalize, conforme exigido por Shape
    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) override;

    // O método draw chama o draw do círculo interno
    void draw(SDL_Surface* surface) override;
};

#endif

