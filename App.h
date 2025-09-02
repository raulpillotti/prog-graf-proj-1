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

public:
    App(const std::string& title, int w, int h);
    ~App();

    void run();
    void handleEvents();
    void clear(Uint8 r, Uint8 g, Uint8 b);
    void present();

    SDL_Surface* getSurface() { return surface; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif
