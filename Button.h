#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>

#include "Rectangle.h"

class Button : public Rectangle {
private:
    std::string label;
    TTF_Font* font;
    Uint32 textColor;
    std::function<void()> onClick;

public:
    Button(int x1, int y1, int x2, int y2, Uint32 color,
           const std::string& label, TTF_Font* font, Uint32 textColor,
           std::function<void()> onClick = nullptr)
        : Rectangle(x1, y1, x2, y2, color),
          label(label),
          font(font),
          textColor(textColor),
          onClick(onClick) {}

    void setCallback(std::function<void()> func) {
        onClick = func;
    }

    void draw(SDL_Surface* surface) override {
        // Desenha o retângulo de fundo
        Rectangle::draw(surface);

        std::cout << "Passou 1" << std::endl;

        if (!font || label.empty()) return;

        std::cout << "Passou 2" << std::endl;

        SDL_Color cor_texto = {0, 0, 0, 255};

        std::cout << label << std::endl;


        // Renderiza o texto
        /*
        //SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, label.c_str(), Primitives::uint32ToSDL_COLOR(surface,textColor));
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, label.c_str(), cor_texto);
        std::cout << "Passou 2.5" << std::endl;

        if (!textSurface) return;

        std::cout << "Passou 3" << std::endl;

        // Centraliza o texto no botão
        int btnW = getX2() - getX1();
        int btnH = getY2() - getY1();

        int textX = getX1() + (btnW - textSurface->w) / 2;
        int textY = getY1() + (btnH - textSurface->h) / 2;

        SDL_Rect dstRect = { textX, textY, textSurface->w, textSurface->h };
        SDL_BlitSurface(textSurface, nullptr, surface, &dstRect);

        SDL_FreeSurface(textSurface);
        */
    }

    void handleEvent(const SDL_Event& e) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x;
            int my = e.button.y;
            if (mx >= getX1() && mx <= getX2() && my >= getY1() && my <= getY2()) {
                if (onClick) onClick();
            }
        }
    }
};

#endif
