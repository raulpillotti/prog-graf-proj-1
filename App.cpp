#include "App.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Polygon.h"
#include "Point.h"
#include "Primitives.h"


Uint32 parseColor(const std::string& colorStr);

App::App(const std::string& title, int w, int h)
    : window(nullptr), surface(nullptr), running(false),
      width(w), height(h), title(title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    surface = SDL_GetWindowSurface(window);
}

App::~App() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void App::run() {
    running = true;
    parseCSV("Exemplo.csv");
    normalizeScene();

    while (running) {
        handleEvents();
        drawScene();
        present();
        SDL_Delay(16);
    }
}

void App::normalizeScene() {
    for(auto& house : houses) house.normalize(metersX, metersY, width, height);
    for(auto& tree : trees) tree.normalize(metersX, metersY, width, height);
    for(auto& fence : fences) fence.normalize(metersX, metersY, width, height);
    for(auto& sun : suns) sun.normalize(metersX, metersY, width, height);
}

void App::drawScene() {
    SDL_FillRect(surface, nullptr, bgColor);
    for (auto& house : houses) house.draw(surface);
    for (auto& tree : trees) tree.draw(surface);
    for (auto& fence : fences) fence.draw(surface);
    for (auto& sun : suns) sun.draw(surface);
}

void App::parseCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string currentObjectType = "None";

    float x = 0, y = 0, width = 0, height = 0, inclination = 0;
    Uint32 c1 = 0xFFFFFF, c2 = 0xFFFFFF, c3 = 0xFFFFFF;

    auto buildCurrentObject = [&]() {
        if (currentObjectType == "Casa") {
            houses.emplace_back();
            houses.back().build(x, y - height, width, height, c1, c2, c3);
        } else if (currentObjectType == "Arvore") {
            trees.emplace_back();
            trees.back().build(x, y - height, width, height, c1, c2, inclination);
        } else if (currentObjectType == "Cerca") {
            fences.emplace_back();
            fences.back().build(x, y - height, width, height, c1, inclination);
        } else if (currentObjectType == "Sol") {
            suns.emplace_back();
            suns.back().build(x, y - height, width, height, c1);
        }
        x = y = width = height = inclination = 0;
        c1 = c2 = c3 = 0xFFFFFF;
        currentObjectType = "None";
    };

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }
        if (tokens.empty()) continue;

        const std::string& key = tokens[0];

        if (key == "Casa" || key == "Arvore" || key == "Cerca" || key == "Sol") {
            buildCurrentObject();
            currentObjectType = key;
            continue;
        }

        if (key.rfind("Cor:", 0) == 0 && currentObjectType == "Sol") {
            c1 = parseColor(key);
            continue;
        }

        if (tokens.size() < 2) continue;
        const std::string& value = tokens[1];

        if (key == "Resolucao" && tokens.size() >= 3) setResolution(std::stoi(value), std::stoi(tokens[2]));
        else if (key == "Metros" && tokens.size() >= 3) setMeters(std::stof(value), std::stof(tokens[2]));
        else if (key == "Cor" && currentObjectType == "None") setBgColor(parseColor(value));
        else if (key == "Localizacao" && tokens.size() >= 3) {
            x = std::stof(value);
            y = std::stof(tokens[2]);
            // float y1 = metersY - std::stof(tokens[2]);
            // printf("y: %f %f\n", y1, y);
        }
        else if (key == "Altura") height = std::stof(value);
        else if (key == "Largura") width = std::stof(value);
        else if (key == "Inclinacao") inclination = std::stof(value);
        else if (currentObjectType == "Casa") {
            if (key == "CorParede") c1 = parseColor(value);
            else if (key == "CorTelhado") c2 = parseColor(value);
            else if (key == "CorPorta") c3 = parseColor(value);
        } else if (currentObjectType == "Arvore") {
            if (key == "CorTronco") c1 = parseColor(value);
            else if (key == "CorFolhas") c2 = parseColor(value);
        } else if (currentObjectType == "Cerca") {
            if (key == "Cor") c1 = parseColor(value);
        }
    }
    buildCurrentObject();
    file.close();
}

void App::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_RESIZED) {
            surface = SDL_GetWindowSurface(window);
            width = surface->w;
            height = surface->h;
            normalizeScene();
        }
    }
}

void App::present() {
    SDL_UpdateWindowSurface(window);
}

void App::setResolution(int w, int h) {
    width = w;
    height = h;
    if (window) {
        SDL_SetWindowSize(window, w, h);
        surface = SDL_GetWindowSurface(window);
    }
}

void App::setMeters(float mx, float my) {
    metersX = mx;
    metersY = my;
    std::cout << "metersX = " << metersX << ", metersY = " << metersY << std::endl;
}

void App::setBgColor(Uint32 color) {
    bgColor = color;
}

Uint32 parseColor(const std::string& colorStr) {
    std::string color = colorStr;
    size_t pos = color.find(":");
    if (pos != std::string::npos) {
        color = color.substr(pos + 1);
    }
    if (color == "Amarelo" || color == "Amarela") return 0xFFFF00;
    if (color == "Vermelho") return 0xFF0000;
    if (color == "Marrom") return 0x8B4513;
    if (color == "Verde") return 0x00FF00;
    if (color == "Lima") return 0x32CD32;
    if (color == "Azul") return 0x00BFFF;
    if (color == "Laranja") return 0xFFA500;
    return 0xFFFFFF;
}
