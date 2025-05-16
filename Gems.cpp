#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "GameBoard.hpp"
#include "GameConfig.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    if (!Config::load("config.json")) {
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
        return 1;
    }

    const auto& windowConfig = Config::getWindow();

    SDL_Window* window = SDL_CreateWindow(
        windowConfig.title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowConfig.width,
        windowConfig.height,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Surface* icon = SDL_LoadBMP("icon.bmp");
    if (icon == nullptr) {
        std::cerr << "Failed to load icon" << std::endl;
    } else {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    const auto& gameConfig = Config::getGame();
    GameBoard gameBoard(gameConfig.boardSize, gameConfig.bombChance, gameConfig.recolorChance);
    
    bool isRunning = true;
    auto lastTime = std::chrono::high_resolution_clock::now();

    enum class GameState {
        PLAYING,
        ANIMATING
    };
    GameState currentState = GameState::PLAYING;

    while (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && 
                      event.button.button == SDL_BUTTON_LEFT && 
                      currentState == GameState::PLAYING) {
                gameBoard.handleClick(event.button.x, event.button.y, windowConfig.width, windowConfig.height);
            }
        }

        gameBoard.update(deltaTime);

        switch (currentState) {
            case GameState::PLAYING: {
                auto matches = gameBoard.checkMatches();
                if (!matches.empty()) {
                    gameBoard.clearMatchedTiles(matches);
                    currentState = GameState::ANIMATING;
                }
                break;
            }
            case GameState::ANIMATING: {
                if (!gameBoard.isAnimating()) {
                    gameBoard.fillEmptySpaces(windowConfig.width, windowConfig.height);
                    currentState = GameState::PLAYING;
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        gameBoard.render(renderer, windowConfig.width, windowConfig.height);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}