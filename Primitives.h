#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <SDL2/SDL.h>

class Primitives {
public:
    // Pixel "base" — usado por todos os outros algoritmos
    static void setPixel(SDL_Surface* surface, int x, int y, Uint32 color);

    // Função para pegar a cor de um pixel na superfície
    static Uint32 getPixel(SDL_Surface* surface, int x, int y);

    // Função para converter valores RGB em Uint32
    static Uint32 rgbToUint32(SDL_Surface* surface, int r, int g, int b);

    // Retas
    static void drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color);
    static void drawLineAA(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color);

    // Círculo
    static void drawCircle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 color);

    // Curvas de Bézier (exemplo: quadrática)
    static void drawBezier(SDL_Surface* surface, int x0, int y0, int x1, int y1, int x2, int y2, Uint32 color);

    // Flood fill (4-direções)
    static void floodFill(SDL_Surface* surface, int x, int y, Uint32 fillColor, Uint32 borderColor);

    // Flood fill
    static void floodFill(SDL_Surface* surface, int x, int y, Uint32 fillColor);

    // Converte uma cor RGBA para padrão SDL
    static SDL_Color uint32ToSDL_COLOR(SDL_Surface* surface, Uint32 color);

};

#endif
