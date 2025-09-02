#include "Primitives.h"
#include <cmath>
#include <stack>

void Primitives::setPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    if (!surface) return;
    if (x < 0 || y < 0 || x >= surface->w || y >= surface->h) return;

    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / 4;
    pixels[y * pitch + x] = color;
}

// ===============================================
// Pega a cor do pixel (x,y) na superfície
// ===============================================
Uint32 Primitives::getPixel(SDL_Surface* surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16*)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32*)p;
        default:
            return 0; // Nunca deveria chegar aqui
    }
}

// ===============================================
// Converte (R,G,B) → Uint32 (com base no formato da surface)
// ===============================================
Uint32 Primitives::rgbToUint32(SDL_Surface* surface, int r, int g, int b) {
    return SDL_MapRGB(surface->format, r, g, b);
}

// Algoritmo de Bresenham
void Primitives::drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        setPixel(surface, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

// Anti-aliased line (Wu's algorithm simplificado)
void Primitives::drawLineAA(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color) {
    // (Implementação simplificada — posso detalhar depois)
    drawLine(surface, x1, y1, x2, y2, color);
}

// Algoritmo do círculo (Midpoint circle algorithm)
void Primitives::drawCircle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 color) {
    int x = radius, y = 0, err = 0;

    while (x >= y) {
        setPixel(surface, cx + x, cy + y, color);
        setPixel(surface, cx + y, cy + x, color);
        setPixel(surface, cx - y, cy + x, color);
        setPixel(surface, cx - x, cy + y, color);
        setPixel(surface, cx - x, cy - y, color);
        setPixel(surface, cx - y, cy - x, color);
        setPixel(surface, cx + y, cy - x, color);
        setPixel(surface, cx + x, cy - y, color);

        y += 1;
        if (err <= 0) {
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

// Bézier quadrática
void Primitives::drawBezier(SDL_Surface* surface, int x0, int y0, int x1, int y1, int x2, int y2, Uint32 color) {
    const int STEPS = 100;
    for (int i = 0; i <= STEPS; i++) {
        float t = i / (float)STEPS;
        float xt = (1-t)*(1-t)*x0 + 2*(1-t)*t*x1 + t*t*x2;
        float yt = (1-t)*(1-t)*y0 + 2*(1-t)*t*y1 + t*t*y2;
        setPixel(surface, (int)xt, (int)yt, color);
    }
}

// Flood-fill (pilha iterativa)
void Primitives::floodFill(SDL_Surface* surface, int x, int y, Uint32 fillColor, Uint32 borderColor) {
    if (!surface) return;
    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / 4;

    auto getPixel = [&](int px, int py) -> Uint32 {
        return pixels[py * pitch + px];
    };

    std::stack<std::pair<int,int>> stack;
    stack.push({x,y});

    while (!stack.empty()) {
        auto [px, py] = stack.top();
        stack.pop();

        if (px < 0 || py < 0 || px >= surface->w || py >= surface->h) continue;

        if (getPixel(px, py) != borderColor && getPixel(px, py) != fillColor) {
            setPixel(surface, px, py, fillColor);
            stack.push({px+1, py});
            stack.push({px-1, py});
            stack.push({px, py+1});
            stack.push({px, py-1});
        }
    }
}

// ===============================================
// Flood fill (algoritmo de preenchimento)
// ===============================================
void Primitives::floodFill(SDL_Surface* surface, int x, int y, Uint32 fillColor) {
    Uint32 targetColor = getPixel(surface, x, y);
    if (targetColor == fillColor) return;

    std::stack<std::pair<int,int>> pixels;
    pixels.push({x,y});

    while (!pixels.empty()) {
        auto [px, py] = pixels.top();
        pixels.pop();

        if (px < 0 || px >= surface->w || py < 0 || py >= surface->h) continue;
        if (getPixel(surface, px, py) != targetColor) continue;

        Uint32* pixelAddr = (Uint32*)((Uint8*)surface->pixels + py * surface->pitch + px * 4);
        *pixelAddr = fillColor;

        pixels.push({px+1, py});
        pixels.push({px-1, py});
        pixels.push({px, py+1});
        pixels.push({px, py-1});
    }
}

// ===============================================
// Converte de Uint32 para SDL_COLOR
// ===============================================
SDL_Color Primitives::uint32ToSDL_COLOR(SDL_Surface* surface, Uint32 color) {

    SDL_Color sdlColor;
    SDL_GetRGBA(color, surface->format,
            &sdlColor.r, &sdlColor.g, &sdlColor.b, &sdlColor.a);
    return sdlColor;
}
