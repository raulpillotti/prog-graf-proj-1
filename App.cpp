#include "App.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


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
    parseCSV("Entrega(1).csv");
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
            houses.back().build(x, y, width, height, c1, c2, c3, inclination);
        } else if (currentObjectType == "Arvore") {
            trees.emplace_back();
            trees.back().build(x, y, width, height, c1, c2, inclination);
        } else if (currentObjectType == "Cerca") {
            fences.emplace_back();
            fences.back().build(x, y, width, height, c1, inclination);
        } else if (currentObjectType == "Sol") {
            suns.emplace_back();
            suns.back().build(x, y, width, height, c1);
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
        } else if (currentObjectType == "Sol") {
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

    // std::transform(color.begin(), color.end(), color.begin(), ::tolower);

    if (color == "amarelo" || color == "amarela") return 0xFFFF00;
    if (color == "vermelho") return 0xFF0000;
    if (color == "marrom")   return 0x8B4513;
    if (color == "verde")    return 0x00FF00;
    if (color == "lima")     return 0x32CD32;
    if (color == "azul")     return 0x00BFFF;
    if (color == "laranja")  return 0xFFA500;

    if (color == "aliceblue") return 0xF0F8FF;
    if (color == "antiquewhite") return 0xFAEBD7;
    if (color == "aqua") return 0x00FFFF;
    if (color == "aquamarine") return 0x7FFFD4;
    if (color == "azure") return 0xF0FFFF;
    if (color == "beige") return 0xF5F5DC;
    if (color == "bisque") return 0xFFE4C4;
    if (color == "black") return 0x000000;
    if (color == "blanchedalmond") return 0xFFEBCD;
    if (color == "blue") return 0x0000FF;
    if (color == "blueviolet") return 0x8A2BE2;
    if (color == "brown") return 0xA52A2A;
    if (color == "burlywood") return 0xDEB887;
    if (color == "cadetblue") return 0x5F9EA0;
    if (color == "chartreuse") return 0x7FFF00;
    if (color == "chocolate") return 0xD2691E;
    if (color == "coral") return 0xFF7F50;
    if (color == "cornflowerblue") return 0x6495ED;
    if (color == "cornsilk") return 0xFFF8DC;
    if (color == "crimson") return 0xDC143C;
    if (color == "cyan") return 0x00FFFF;
    if (color == "darkblue") return 0x00008B;
    if (color == "darkcyan") return 0x008B8B;
    if (color == "darkgoldenrod") return 0xB8860B;
    if (color == "darkgray") return 0xA9A9A9;
    if (color == "darkgreen") return 0x006400;
    if (color == "darkkhaki") return 0xBDB76B;
    if (color == "darkmagenta") return 0x8B008B;
    if (color == "darkolivegreen") return 0x556B2F;
    if (color == "darkorange") return 0xFF8C00;
    if (color == "darkorchid") return 0x9932CC;
    if (color == "darkred") return 0x8B0000;
    if (color == "darksalmon") return 0xE9967A;
    if (color == "darkseagreen") return 0x8FBC8F;
    if (color == "darkslateblue") return 0x483D8B;
    if (color == "darkslategray") return 0x2F4F4F;
    if (color == "darkturquoise") return 0x00CED1;
    if (color == "darkviolet") return 0x9400D3;
    if (color == "deeppink") return 0xFF1493;
    if (color == "deepskyblue") return 0x00BFFF;
    if (color == "dimgray") return 0x696969;
    if (color == "dodgerblue") return 0x1E90FF;
    if (color == "firebrick") return 0xB22222;
    if (color == "floralwhite") return 0xFFFAF0;
    if (color == "forestgreen") return 0x228B22;
    if (color == "fuchsia") return 0xFF00FF;
    if (color == "gainsboro") return 0xDCDCDC;
    if (color == "ghostwhite") return 0xF8F8FF;
    if (color == "gold") return 0xFFD700;
    if (color == "goldenrod") return 0xDAA520;
    if (color == "gray") return 0x808080;
    if (color == "green") return 0x008000;
    if (color == "greenyellow") return 0xADFF2F;
    if (color == "honeydew") return 0xF0FFF0;
    if (color == "hotpink") return 0xFF69B4;
    if (color == "indianred") return 0xCD5C5C;
    if (color == "indigo") return 0x4B0082;
    if (color == "ivory") return 0xFFFFF0;
    if (color == "khaki") return 0xF0E68C;
    if (color == "lavender") return 0xE6E6FA;
    if (color == "lavenderblush") return 0xFFF0F5;
    if (color == "lawngreen") return 0x7CFC00;
    if (color == "lemonchiffon") return 0xFFFACD;
    if (color == "lightblue") return 0xADD8E6;
    if (color == "lightcoral") return 0xF08080;
    if (color == "lightcyan") return 0xE0FFFF;
    if (color == "lightgoldenrodyellow") return 0xFAFAD2;
    if (color == "lightgray") return 0xD3D3D3;
    if (color == "lightgreen") return 0x90EE90;
    if (color == "lightpink") return 0xFFB6C1;
    if (color == "lightsalmon") return 0xFFA07A;
    if (color == "lightseagreen") return 0x20B2AA;
    if (color == "lightskyblue") return 0x87CEFA;
    if (color == "lightslategray") return 0x778899;
    if (color == "lightsteelblue") return 0xB0C4DE;
    if (color == "lightyellow") return 0xFFFFE0;
    if (color == "lime") return 0x00FF00;
    if (color == "limegreen") return 0x32CD32;
    if (color == "linen") return 0xFAF0E6;
    if (color == "magenta") return 0xFF00FF;
    if (color == "maroon") return 0x800000;
    if (color == "mediumaquamarine") return 0x66CDAA;
    if (color == "mediumblue") return 0x0000CD;
    if (color == "mediumorchid") return 0xBA55D3;
    if (color == "mediumpurple") return 0x9370DB;
    if (color == "mediumseagreen") return 0x3CB371;
    if (color == "mediumslateblue") return 0x7B68EE;
    if (color == "mediumspringgreen") return 0x00FA9A;
    if (color == "mediumturquoise") return 0x48D1CC;
    if (color == "mediumvioletred") return 0xC71585;
    if (color == "midnightblue") return 0x191970;
    if (color == "mintcream") return 0xF5FFFA;
    if (color == "mistyrose") return 0xFFE4E1;
    if (color == "moccasin") return 0xFFE4B5;
    if (color == "navajowhite") return 0xFFDEAD;
    if (color == "navy") return 0x000080;
    if (color == "oldlace") return 0xFDF5E6;
    if (color == "olive") return 0x808000;
    if (color == "olivedrab") return 0x6B8E23;
    if (color == "orange") return 0xFFA500;
    if (color == "orangered") return 0xFF4500;
    if (color == "orchid") return 0xDA70D6;
    if (color == "palegoldenrod") return 0xEEE8AA;
    if (color == "palegreen") return 0x98FB98;
    if (color == "paleturquoise") return 0xAFEEEE;
    if (color == "palevioletred") return 0xDB7093;
    if (color == "papayawhip") return 0xFFEFD5;
    if (color == "peachpuff") return 0xFFDAB9;
    if (color == "peru") return 0xCD853F;
    if (color == "pink") return 0xFFC0CB;
    if (color == "plum") return 0xDDA0DD;
    if (color == "powderblue") return 0xB0E0E6;
    if (color == "purple") return 0x800080;
    if (color == "rebeccapurple") return 0x663399;
    if (color == "red") return 0xFF0000;
    if (color == "rosybrown") return 0xBC8F8F;
    if (color == "royalblue") return 0x4169E1;
    if (color == "saddlebrown") return 0x8B4513;
    if (color == "salmon") return 0xFA8072;
    if (color == "sandybrown") return 0xF4A460;
    if (color == "seagreen") return 0x2E8B57;
    if (color == "seashell") return 0xFFF5EE;
    if (color == "sienna") return 0xA0522D;
    if (color == "silver") return 0xC0C0C0;
    if (color == "skyblue") return 0x87CEEB;
    if (color == "slateblue") return 0x6A5ACD;
    if (color == "slategray") return 0x708090;
    if (color == "snow") return 0xFFFAFA;
    if (color == "springgreen") return 0x00FF7F;
    if (color == "steelblue") return 0x4682B4;
    if (color == "tan") return 0xD2B48C;
    if (color == "teal") return 0x008080;
    if (color == "thistle") return 0xD8BFD8;
    if (color == "tomato") return 0xFF6347;
    if (color == "turquoise") return 0x40E0D0;
    if (color == "violet") return 0xEE82EE;
    if (color == "wheat") return 0xF5DEB3;
    if (color == "white") return 0xFFFFFF;
    if (color == "whitesmoke") return 0xF5F5F5;
    if (color == "yellow") return 0xFFFF00;
    if (color == "yellowgreen") return 0x9ACD32;

    return 0xFFFFFF;
}
