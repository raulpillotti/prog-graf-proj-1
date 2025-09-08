#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "House.h"
#include "Tree.h"
#include "Fence.h"
#include "Sun.h"

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

    std::vector<House> houses;
    std::vector<Tree> trees;
    std::vector<Fence> fences;
    std::vector<Sun> suns;

    void handleEvents();
    void normalizeScene(); 
    void drawScene();
    void parseCSV(const std::string& filename); 

public:
    App(const std::string& title, int w, int h);
    ~App();

    void run();
    void present();

    void setResolution(int w, int h);
    void setMeters(float mx, float my);
    void setBgColor(Uint32 c);
};

#endif

