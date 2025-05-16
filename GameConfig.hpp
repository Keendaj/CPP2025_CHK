#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

struct WindowConfig {
    int width;
    int height;
    std::string title;
};

struct GameConfig {
    int boardSize;
    double bombChance;
    double recolorChance;
};

struct AnimationConfig {
    double scaleSpeed;
    double alphaSpeed;
};

class Config {
    public:
        static bool load(const std::string& filename);
        
        static const WindowConfig& getWindow() { return window; }
        static const GameConfig& getGame() { return game; }
        static const AnimationConfig& getAnimation() { return animation; }
        static const std::vector<SDL_Color>& getColors() { return colors; }

    private:
        static WindowConfig window;
        static GameConfig game;
        static AnimationConfig animation;
        static std::vector<SDL_Color> colors;
}; 