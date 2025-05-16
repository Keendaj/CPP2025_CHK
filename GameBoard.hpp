#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <memory>
#include <set>
#include "Tile.hpp"

struct Animation {
    std::pair<int, int> pos;
    double scale;
    double alpha;
    bool active;
};

class GameBoard {
    private:
        std::vector<std::vector<std::unique_ptr<Tile>>> board;
        std::vector<Animation> animations;
        std::pair<int, int> selectedTile;
        double bombChance;
        double recolorChance;
        
        bool isTileAnimating(int x, int y) const;
        void createBonusTile(std::pair<int, int> pos);
        void handleBombActivation(int x, int y);
        void handleRecolorActivation(int x, int y);
        std::vector<std::pair<int, int>> checkLine(std::pair<int, int> coords, bool vertical) const;

    public:
        GameBoard(int size, double bombChance, double recolorChance);
        
        void update(double deltaTime);
        void render(SDL_Renderer* renderer, int screenWidth, int screenHeight) const;
        void handleClick(int mouseX, int mouseY, int screenWidth, int screenHeight);
        void fillEmptySpaces(int width, int height);
        std::set<std::pair<int, int>> checkMatches() const;
        void clearMatchedTiles(const std::set<std::pair<int, int>>& matches);
        bool isAnimating() const { return !animations.empty(); }
}; 