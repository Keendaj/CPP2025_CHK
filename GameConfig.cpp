#include "GameConfig.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
WindowConfig Config::window;
GameConfig Config::game;
AnimationConfig Config::animation;
std::vector<SDL_Color> Config::colors;

bool Config::load(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open configuration file: " << filename << std::endl;
            return false;
        }

        json j;
        file >> j;

        window.width = j["window"]["width"];
        window.height = j["window"]["height"];
        window.title = j["window"]["title"];

        game.boardSize = j["game"]["board_size"];
        game.bombChance = j["game"]["bomb_chance"];
        game.recolorChance = j["game"]["recolor_chance"];

        animation.scaleSpeed = j["animation"]["scale_speed"];
        animation.alphaSpeed = j["animation"]["alpha_speed"];

        colors.clear();
        for (const auto& color : j["colors"]) {
            colors.push_back({
                static_cast<Uint8>(color["r"]),
                static_cast<Uint8>(color["g"]),
                static_cast<Uint8>(color["b"]),
                static_cast<Uint8>(color["a"])
            });
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading configuration file: " << e.what() << std::endl;
        return false;
    }
} 