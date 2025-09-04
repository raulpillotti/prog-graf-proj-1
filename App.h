#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

class App {
private:
    SDL_Window* window;
    SDL_Surface* surface;
    bool running;
    int width, height;
    std::string title;
    float metersX = 0.0f;
    float metersY = 0.0f;
    Uint32 bgColor = 0xFFFFFF;

public:
    App(const std::string& title, int w, int h);
    ~App();

    void run();
    void handleEvents();
    void clear(Uint8 r, Uint8 g, Uint8 b);
    void present();

    SDL_Surface* getSurface() const { return surface; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    float getMetersX() const { return metersX; }
    float getMetersY() const { return metersY; }
    Uint32 getBgColor() const { return bgColor; }

    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    void setMetersX(float mx) { metersX = mx; }
    void setMetersY(float my) { metersY = my; }

    void setResolution(int w, int h);
    void setMeters(float mx, float my);
    void setBgColor(Uint32 c);
};

#endif
