#include "Tile.hpp"
#include "GameConfig.hpp"

void Tile::render(SDL_Renderer* renderer, const SDL_Rect& bounds, Uint8 alpha) const {
    double scale = selected ? 0.8 : 0.9;
    
    SDL_Rect scaledRect = {
        bounds.x + static_cast<int>(bounds.w * (1 - scale) / 2),
        bounds.y + static_cast<int>(bounds.h * (1 - scale) / 2),
        static_cast<int>(bounds.w * scale),
        static_cast<int>(bounds.h * scale)
    };
    
    const auto& configColors = Config::getColors();
    SDL_Color tileColor = configColors[color];
    SDL_SetRenderDrawColor(renderer, tileColor.r, tileColor.g, tileColor.b, alpha);
    SDL_RenderFillRect(renderer, &scaledRect);
}

void BasicTile::render(SDL_Renderer* renderer, const SDL_Rect& bounds, Uint8 alpha) const {
    Tile::render(renderer, bounds, alpha);
}

void BombTile::render(SDL_Renderer* renderer, const SDL_Rect& bounds, Uint8 alpha) const {
    Tile::render(renderer, bounds, alpha);
    
    double indicatorScale = 0.3;
    SDL_Rect indicator = {
        bounds.x + static_cast<int>(bounds.w * (1 - indicatorScale) / 2),
        bounds.y + static_cast<int>(bounds.h * (1 - indicatorScale) / 2),
        static_cast<int>(bounds.w * indicatorScale),
        static_cast<int>(bounds.h * indicatorScale)
    };
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
    SDL_RenderFillRect(renderer, &indicator);
}

void RecolorTile::render(SDL_Renderer* renderer, const SDL_Rect& bounds, Uint8 alpha) const {
    Tile::render(renderer, bounds, alpha);

    double indicatorScale = 0.3;
    SDL_Rect indicator = {
        bounds.x + static_cast<int>(bounds.w * (1 - indicatorScale) / 2),
        bounds.y + static_cast<int>(bounds.h * (1 - indicatorScale) / 2),
        static_cast<int>(bounds.w * indicatorScale),
        static_cast<int>(bounds.h * indicatorScale)
    };
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
    SDL_RenderFillRect(renderer, &indicator);
}

void BombTile::activate() {
    activated = true;
}

void RecolorTile::activate() {
    activated = true;
} 