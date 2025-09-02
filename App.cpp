#include "App.h"
#include "Polygon.h"
#include "Point.h"
#include "Sun.h"
#include "vector"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "House.h"
#include "Tree.h"
#include "Fence.h"
#include "Polygon.h"
#include "Point.h"

void parseCSV(const std::string& filename,
              std::vector<House>& houses,
              std::vector<Tree>& trees,
              std::vector<Fence>& fences,
              std::vector<Sun>& suns);


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
    std::vector<House> houses;
    std::vector<Tree> trees;
    std::vector<Fence> fences;
    std::vector<Sun> suns;

    // Carregar a cena do CSV
    parseCSV("Exemplo.csv", houses, trees, fences, suns);

    // ---- DEBUG: printar os objetos carregados ----
    std::cout << "Casas encontradas: " << houses.size() << std::endl;
    for (int i = 0; i < houses.size(); ++i) {
        std::cout << "Casa " << i
                  << " - Pos(" << houses[i].getWall().getPoints()[0].x
                  << "," << houses[i].getWall().getPoints()[0].y << ")"
                  << std::endl;
    }

    std::cout << "Árvores encontradas: " << trees.size() << std::endl;
    for (int i = 0; i < trees.size(); ++i) {
        std::cout << "Árvore " << i
                  << " - Tronco Pos(" << trees[i].getTrunk().getPoints()[0].x
                  << "," << trees[i].getTrunk().getPoints()[0].y << ")"
                  << std::endl;
    }

    std::cout << "Cercas encontradas: " << fences.size() << std::endl;
    for (int i = 0; i < fences.size(); ++i) {
        std::cout << "Cerca " << i
                  << " - Post Pos(" << fences[i].getPost().getPoints()[0].x
                  << "," << fences[i].getPost().getPoints()[0].y << ")"
                  << std::endl;
    }

    std::cout << "Sois encontradas: " << suns.size() << std::endl;
    for (int i = 0; i < suns.size(); ++i) {
        std::cout << "Sol " << i
                  << " - Post Pos(" << suns[i].getBody().getPoints()[0].x
                  << "," << suns[i].getBody().getPoints()[0].y << ")"
                  << std::endl;
    }

    while (running) {

        // Limpar fundo
        clear(255, 255, 255);

        Uint32 red = Primitives::rgbToUint32(surface, 255, 0, 0);
        Uint32 green = Primitives::rgbToUint32(surface, 0, 255, 0);
        Uint32 blue = Primitives::rgbToUint32(surface, 0, 0, 255);
        Uint32 black = Primitives::rgbToUint32(surface, 0, 0, 0);
        Point p1(10, 32);
        Point p2(30, 65);
        Point p3(100, 21);
        Point p4(46, 90);

        std::vector<Point> points = {p1, p2, p3, p4};

        Polygon pol(points, blue);
        pol.draw(surface);

        // Criar um retângulo vermelho
       // Rectangle rect(50, 50, 200, 150, red);
        //rect.draw(surface);

        //Circle circ(300, 300, 100, black);
        //circ.draw(surface);

        //CircleFilled circf(300, 300, 70, green);
        //circf.draw(surface);

        //CircleFilledAA circfa(500, 100, 70, blue);
        //circfa.draw(surface);

        handleEvents();

        // Atualizar tela
        present();
    }
}

Uint32 parseColor(const std::string& color) {
    if (color == "Amarelo") return 0xFFFF00;
    if (color == "Vermelho") return 0xFF0000;
    if (color == "Marrom") return 0x8B4513;
    if (color == "Verde") return 0x00FF00;
    if (color == "Lima") return 0x32CD32;
    if (color == "Azul") return 0x0000FF;
    if (color == "Laranja") return 0xFFA500;
    return 0xFFFFFF;
}

void parseCSV(const std::string& filename,
                   std::vector<House>& houses,
                   std::vector<Tree>& trees,
                   std::vector<Fence>& fences,
                   std::vector<Sun>& suns) {

    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    House* currentHouse = nullptr;
    Tree* currentTree = nullptr;
    Fence* currentFence = nullptr;
    Sun* currentSun = nullptr; // <--- adicionado

    float x = 0, y = 0, width = 1, height = 1;
    Uint32 color = 0xFFFFFF;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }
        if (tokens.empty()) continue;

        if (tokens[0] == "Casa") {
            houses.push_back(House());
            currentHouse = &houses.back();
            currentTree = nullptr;
            currentFence = nullptr;
            currentSun = nullptr; // <--- reset
        }
        else if (tokens[0] == "Arvore") {
            trees.push_back(Tree());
            currentTree = &trees.back();
            currentHouse = nullptr;
            currentFence = nullptr;
            currentSun = nullptr;
        }
        else if (tokens[0] == "Cerca") {
            fences.push_back(Fence());
            currentFence = &fences.back();
            currentHouse = nullptr;
            currentTree = nullptr;
            currentSun = nullptr;
        }
        else if (tokens[0] == "Sol") {
            suns.push_back(Sun());
            currentSun = &suns.back();
            currentHouse = nullptr;
            currentTree = nullptr;
            currentFence = nullptr;
        }
        else if (tokens[0] == "Localizacao") {
            x = std::stof(tokens[1]);
            y = std::stof(tokens[2]);
        }
        else if (tokens[0] == "Altura") {
            height = std::stof(tokens[1]);
        }
        else if (tokens[0] == "Largura") {
            width = std::stof(tokens[1]);
        }
        else if (tokens[0].find("Cor") != std::string::npos) {
            color = parseColor(tokens[1]);
        }

        if (currentHouse) {
            currentHouse->getWall().setPoints({{x, y}, {x+width, y}, {x+width, y-height}, {x, y-height}});
            currentHouse->getWall().setColor(color);
            currentHouse->getRoof().setPoints({{x-0.1f*width, y-height}, {x+1.1f*width, y-height}, {x+width/2, y-height-height/2}});
            currentHouse->getRoof().setColor(color);
            currentHouse->getDoor().setPoints({{x+0.4f*width, y}, {x+0.6f*width, y}, {x+0.6f*width, y-0.5f*height}, {x+0.4f*width, y-0.5f*height}});
            currentHouse->getDoor().setColor(color);
        }
        else if (currentTree) {
            currentTree->getTrunk().setPoints({{x, y}, {x+width*0.3f, y}, {x+width*0.3f, y-height*0.5f}, {x, y-height*0.5f}});
            currentTree->getTrunk().setColor(color);
            currentTree->getLeaves().setPoints({{x-width*0.2f, y-height*0.5f}, {x+width*0.5f, y-height*0.5f}, {x+width*0.15f, y-height}});
            currentTree->getLeaves().setColor(color);
        }
        else if (currentFence) {
            currentFence->getPost().setPoints({{x, y}, {x+width, y}, {x+width, y-height}, {x, y-height}});
            currentFence->getPost().setColor(color);
        }
        else if (currentSun) {
            currentSun->getBody().setPoints({{x, y}, {x+width, y}, {x+width, y-height}, {x, y-height}});
            currentSun->getBody().setColor(color);
        }
    }
}


void App::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            surface = SDL_GetWindowSurface(window);
            width = surface->w;
            height = surface->h;
            std::cout << "Size changed: " << width << ", " << height << "\n";
        }

    }
}

void App::clear(Uint8 r, Uint8 g, Uint8 b) {
    Uint32 color = SDL_MapRGB(surface->format, r, g, b);
    SDL_FillRect(surface, NULL, color);
}

void App::present() {
    SDL_UpdateWindowSurface(window);
}
