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

    return 0xFFFFFF;
}

// Uint32 parseColor(const std::string& colorStr) {
//     std::string color = colorStr;
//     size_t pos = color.find(":");
//     if (pos != std::string::npos) {
//         color = color.substr(pos + 1);
//     }
//     if (color == "Amarelo" || color == "Amarela") return 0xFFFF00;
//     if (color == "Vermelho") return 0xFF0000;
//     if (color == "Marrom") return 0x8B4513;
//     if (color == "Verde") return 0x00FF00;
//     if (color == "Lima") return 0x32CD32;
//     if (color == "Azul") return 0x00BFFF;
//     if (color == "Laranja") return 0xFFA500;

//     std::unordered_map<std::string, RGBA> HtmlColor::colorMap = {
//         {"aliceblue", HtmlColor::hexToRGBA("#F0F8FF")},
//         {"antiquewhite", HtmlColor::hexToRGBA("#FAEBD7")},
//         {"aqua", HtmlColor::hexToRGBA("#00FFFF")},
//         {"aquamarine", HtmlColor::hexToRGBA("#7FFFD4")},
//         {"azure", HtmlColor::hexToRGBA("#F0FFFF")},
//         {"beige", HtmlColor::hexToRGBA("#F5F5DC")},
//         {"bisque", HtmlColor::hexToRGBA("#FFE4C4")},
//         {"black", HtmlColor::hexToRGBA("#000000")},
//         {"blanchedalmond", HtmlColor::hexToRGBA("#FFEBCD")},
//         {"blue", HtmlColor::hexToRGBA("#0000FF")},
//         {"blueviolet", HtmlColor::hexToRGBA("#8A2BE2")},
//         {"brown", HtmlColor::hexToRGBA("#A52A2A")},
//         {"burlywood", HtmlColor::hexToRGBA("#DEB887")},
//         {"cadetblue", HtmlColor::hexToRGBA("#5F9EA0")},
//         {"chartreuse", HtmlColor::hexToRGBA("#7FFF00")},
//         {"chocolate", HtmlColor::hexToRGBA("#D2691E")},
//         {"coral", HtmlColor::hexToRGBA("#FF7F50")},
//         {"cornflowerblue", HtmlColor::hexToRGBA("#6495ED")},
//         {"cornsilk", HtmlColor::hexToRGBA("#FFF8DC")},
//         {"crimson", HtmlColor::hexToRGBA("#DC143C")},
//         {"cyan", HtmlColor::hexToRGBA("#00FFFF")},
//         {"darkblue", HtmlColor::hexToRGBA("#00008B")},
//         {"darkcyan", HtmlColor::hexToRGBA("#008B8B")},
//         {"darkgoldenrod", HtmlColor::hexToRGBA("#B8860B")},
//         {"darkgray", HtmlColor::hexToRGBA("#A9A9A9")},
//         {"darkgreen", HtmlColor::hexToRGBA("#006400")},
//         {"darkkhaki", HtmlColor::hexToRGBA("#BDB76B")},
//         {"darkmagenta", HtmlColor::hexToRGBA("#8B008B")},
//         {"darkolivegreen", HtmlColor::hexToRGBA("#556B2F")},
//         {"darkorange", HtmlColor::hexToRGBA("#FF8C00")},
//         {"darkorchid", HtmlColor::hexToRGBA("#9932CC")},
//         {"darkred", HtmlColor::hexToRGBA("#8B0000")},
//         {"darksalmon", HtmlColor::hexToRGBA("#E9967A")},
//         {"darkseagreen", HtmlColor::hexToRGBA("#8FBC8F")},
//         {"darkslateblue", HtmlColor::hexToRGBA("#483D8B")},
//         {"darkslategray", HtmlColor::hexToRGBA("#2F4F4F")},
//         {"darkturquoise", HtmlColor::hexToRGBA("#00CED1")},
//         {"darkviolet", HtmlColor::hexToRGBA("#9400D3")},
//         {"deeppink", HtmlColor::hexToRGBA("#FF1493")},
//         {"deepskyblue", HtmlColor::hexToRGBA("#00BFFF")},
//         {"dimgray", HtmlColor::hexToRGBA("#696969")},
//         {"dodgerblue", HtmlColor::hexToRGBA("#1E90FF")},
//         {"firebrick", HtmlColor::hexToRGBA("#B22222")},
//         {"floralwhite", HtmlColor::hexToRGBA("#FFFAF0")},
//         {"forestgreen", HtmlColor::hexToRGBA("#228B22")},
//         {"fuchsia", HtmlColor::hexToRGBA("#FF00FF")},
//         {"gainsboro", HtmlColor::hexToRGBA("#DCDCDC")},
//         {"ghostwhite", HtmlColor::hexToRGBA("#F8F8FF")},
//         {"gold", HtmlColor::hexToRGBA("#FFD700")},
//         {"goldenrod", HtmlColor::hexToRGBA("#DAA520")},
//         {"gray", HtmlColor::hexToRGBA("#808080")},
//         {"green", HtmlColor::hexToRGBA("#008000")},
//         {"greenyellow", HtmlColor::hexToRGBA("#ADFF2F")},
//         {"honeydew", HtmlColor::hexToRGBA("#F0FFF0")},
//         {"hotpink", HtmlColor::hexToRGBA("#FF69B4")},
//         {"indianred", HtmlColor::hexToRGBA("#CD5C5C")},
//         {"indigo", HtmlColor::hexToRGBA("#4B0082")},
//         {"ivory", HtmlColor::hexToRGBA("#FFFFF0")},
//         {"khaki", HtmlColor::hexToRGBA("#F0E68C")},
//         {"lavender", HtmlColor::hexToRGBA("#E6E6FA")},
//         {"lavenderblush", HtmlColor::hexToRGBA("#FFF0F5")},
//         {"lawngreen", HtmlColor::hexToRGBA("#7CFC00")},
//         {"lemonchiffon", HtmlColor::hexToRGBA("#FFFACD")},
//         {"lightblue", HtmlColor::hexToRGBA("#ADD8E6")},
//         {"lightcoral", HtmlColor::hexToRGBA("#F08080")},
//         {"lightcyan", HtmlColor::hexToRGBA("#E0FFFF")},
//         {"lightgoldenrodyellow", HtmlColor::hexToRGBA("#FAFAD2")},
//         {"lightgray", HtmlColor::hexToRGBA("#D3D3D3")},
//         {"lightgreen", HtmlColor::hexToRGBA("#90EE90")},
//         {"lightpink", HtmlColor::hexToRGBA("#FFB6C1")},
//         {"lightsalmon", HtmlColor::hexToRGBA("#FFA07A")},
//         {"lightseagreen", HtmlColor::hexToRGBA("#20B2AA")},
//         {"lightskyblue", HtmlColor::hexToRGBA("#87CEFA")},
//         {"lightslategray", HtmlColor::hexToRGBA("#778899")},
//         {"lightsteelblue", HtmlColor::hexToRGBA("#B0C4DE")},
//         {"lightyellow", HtmlColor::hexToRGBA("#FFFFE0")},
//         {"lime", HtmlColor::hexToRGBA("#00FF00")},
//         {"limegreen", HtmlColor::hexToRGBA("#32CD32")},
//         {"linen", HtmlColor::hexToRGBA("#FAF0E6")},
//         {"magenta", HtmlColor::hexToRGBA("#FF00FF")},
//         {"maroon", HtmlColor::hexToRGBA("#800000")},
//         {"mediumaquamarine", HtmlColor::hexToRGBA("#66CDAA")},
//         {"mediumblue", HtmlColor::hexToRGBA("#0000CD")},
//         {"mediumorchid", HtmlColor::hexToRGBA("#BA55D3")},
//         {"mediumpurple", HtmlColor::hexToRGBA("#9370DB")},
//         {"mediumseagreen", HtmlColor::hexToRGBA("#3CB371")},
//         {"mediumslateblue", HtmlColor::hexToRGBA("#7B68EE")},
//         {"mediumspringgreen", HtmlColor::hexToRGBA("#00FA9A")},
//         {"mediumturquoise", HtmlColor::hexToRGBA("#48D1CC")},
//         {"mediumvioletred", HtmlColor::hexToRGBA("#C71585")},
//         {"midnightblue", HtmlColor::hexToRGBA("#191970")},
//         {"mintcream", HtmlColor::hexToRGBA("#F5FFFA")},
//         {"mistyrose", HtmlColor::hexToRGBA("#FFE4E1")},
//         {"moccasin", HtmlColor::hexToRGBA("#FFE4B5")},
//         {"navajowhite", HtmlColor::hexToRGBA("#FFDEAD")},
//         {"navy", HtmlColor::hexToRGBA("#000080")},
//         {"oldlace", HtmlColor::hexToRGBA("#FDF5E6")},
//         {"olive", HtmlColor::hexToRGBA("#808000")},
//         {"olivedrab", HtmlColor::hexToRGBA("#6B8E23")},
//         {"orange", HtmlColor::hexToRGBA("#FFA500")},
//         {"orangered", HtmlColor::hexToRGBA("#FF4500")},
//         {"orchid", HtmlColor::hexToRGBA("#DA70D6")},
//         {"palegoldenrod", HtmlColor::hexToRGBA("#EEE8AA")},
//         {"palegreen", HtmlColor::hexToRGBA("#98FB98")},
//         {"paleturquoise", HtmlColor::hexToRGBA("#AFEEEE")},
//         {"palevioletred", HtmlColor::hexToRGBA("#DB7093")},
//         {"papayawhip", HtmlColor::hexToRGBA("#FFEFD5")},
//         {"peachpuff", HtmlColor::hexToRGBA("#FFDAB9")},
//         {"peru", HtmlColor::hexToRGBA("#CD853F")},
//         {"pink", HtmlColor::hexToRGBA("#FFC0CB")},
//         {"plum", HtmlColor::hexToRGBA("#DDA0DD")},
//         {"powderblue", HtmlColor::hexToRGBA("#B0E0E6")},
//         {"purple", HtmlColor::hexToRGBA("#800080")},
//         {"rebeccapurple", HtmlColor::hexToRGBA("#663399")},
//         {"red", HtmlColor::hexToRGBA("#FF0000")},
//         {"rosybrown", HtmlColor::hexToRGBA("#BC8F8F")},
//         {"royalblue", HtmlColor::hexToRGBA("#4169E1")},
//         {"saddlebrown", HtmlColor::hexToRGBA("#8B4513")},
//         {"salmon", HtmlColor::hexToRGBA("#FA8072")},
//         {"sandybrown", HtmlColor::hexToRGBA("#F4A460")},
//         {"seagreen", HtmlColor::hexToRGBA("#2E8B57")},
//         {"seashell", HtmlColor::hexToRGBA("#FFF5EE")},
//         {"sienna", HtmlColor::hexToRGBA("#A0522D")},
//         {"silver", HtmlColor::hexToRGBA("#C0C0C0")},
//         {"skyblue", HtmlColor::hexToRGBA("#87CEEB")},
//         {"slateblue", HtmlColor::hexToRGBA("#6A5ACD")},
//         {"slategray", HtmlColor::hexToRGBA("#708090")},
//         {"snow", HtmlColor::hexToRGBA("#FFFAFA")},
//         {"springgreen", HtmlColor::hexToRGBA("#00FF7F")},
//         {"steelblue", HtmlColor::hexToRGBA("#4682B4")},
//         {"tan", HtmlColor::hexToRGBA("#D2B48C")},
//         {"teal", HtmlColor::hexToRGBA("#008080")},
//         {"thistle", HtmlColor::hexToRGBA("#D8BFD8")},
//         {"tomato", HtmlColor::hexToRGBA("#FF6347")},
//         {"turquoise", HtmlColor::hexToRGBA("#40E0D0")},
//         {"violet", HtmlColor::hexToRGBA("#EE82EE")},
//         {"wheat", HtmlColor::hexToRGBA("#F5DEB3")},
//         {"white", HtmlColor::hexToRGBA("#FFFFFF")},
//         {"whitesmoke", HtmlColor::hexToRGBA("#F5F5F5")},
//         {"yellow", HtmlColor::hexToRGBA("#FFFF00")},
//         {"yellowgreen", HtmlColor::hexToRGBA("#9ACD32")}
//     };
//     return 0xFFFFFF;
// }
