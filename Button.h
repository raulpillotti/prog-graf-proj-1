#ifndef BUTTON_H
#define BUTTON_H

#include "Rectangle.h"
#include "Primitives.h" 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>
#include <iostream>

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
        : Rectangle(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2), color),
          label(label), font(font), textColor(textColor), onClick(onClick) {}

    void setCallback(std::function<void()> func) {
        onClick = func;
    }


    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) override {

        p1.screen_x = static_cast<int>(p1.world_x);
        p1.screen_y = static_cast<int>(p1.world_y);
        p2.screen_x = static_cast<int>(p2.world_x);
        p2.screen_y = static_cast<int>(p2.world_y);
    }

    void draw(SDL_Surface* surface) override {
        Rectangle::draw(surface);

        if (!font || label.empty()) return;

        SDL_Color sdlTextColor = Primitives::uint32ToSDLColor(surface, textColor);

        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, label.c_str(), sdlTextColor);
        if (!textSurface) {
            std::cerr << "Erro ao renderizar texto: " << TTF_GetError() << std::endl;
            return;
        }

        int btnW = getScreenX2() - getScreenX1();
        int btnH = getScreenY2() - getScreenY1();

        int textX = getScreenX1() + (btnW - textSurface->w) / 2;
        int textY = getScreenY1() + (btnH - textSurface->h) / 2;

        SDL_Rect dstRect = { textX, textY, textSurface->w, textSurface->h };

        SDL_BlitSurface(textSurface, nullptr, surface, &dstRect);

        SDL_FreeSurface(textSurface);
    }

    void handleEvent(const SDL_Event& e) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x;
            int my = e.button.y;
            if (mx >= getScreenX1() && mx <= getScreenX2() && my >= getScreenY1() && my <= getScreenY2()) {
                if (onClick) {
                    onClick();
                }
            }
        }
    }
};

#endif
