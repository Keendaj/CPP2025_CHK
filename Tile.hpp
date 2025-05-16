#pragma once
#include <SDL2/SDL.h>

class Tile {
    protected:
        int color;
        bool selected;

    public:
        Tile(int color) : color(color), selected(false) {}
        virtual ~Tile() = default;

        virtual void render(SDL_Renderer* renderer, const SDL_Rect& bounds, Uint8 alpha = 255) const;
        virtual bool canMatch() const { return true; }
        virtual void onMatch() {}
        
        void setSelected(bool isSelected) { selected = isSelected; }
        int getColor() const { return color; }
        void setColor(int newColor) { color = newColor; }
};

class BasicTile : public Tile {
    public:
        BasicTile(int color) : Tile(color) {}
        void render(SDL_Renderer* renderer, const SDL_Rect& bounds, Uint8 alpha = 255) const override;
};

class BonusTile : public Tile {
    protected:
        bool activated;

    public:
        BonusTile(int color) : Tile(color), activated(false) {}
        virtual void activate() = 0;
        bool isActivated() const { return activated; }
};

class BombTile : public BonusTile {
    public:
        BombTile(int color) : BonusTile(color) {}
        void render(SDL_Renderer* renderer, const SDL_Rect& bounds, Uint8 alpha = 255) const override;
        void activate() override;
};

class RecolorTile : public BonusTile {
    public:
            RecolorTile(int color) : BonusTile(color) {}
            void render(SDL_Renderer* renderer, const SDL_Rect& bounds, Uint8 alpha = 255) const override;
            void activate() override;
}; 