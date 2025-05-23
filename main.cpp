#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Field.hpp"
#include <chrono>
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return 1;
    }
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const int TARGET_FPS = 60;

    SDL_Window* window = SDL_CreateWindow(
        "Arcanoid",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
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

    bool isRunning = true;
    auto lastTime = std::chrono::high_resolution_clock::now();
    std::unique_ptr<Slider> slider = std::make_unique<Slider>(std::make_pair(WINDOW_WIDTH / 2.0f - 50, WINDOW_HEIGHT - 50),
        std::make_pair(100, 20), 600.0f);
    std::unique_ptr<Ball> ball = std::make_unique<Ball>(
        std::make_pair(slider.get()->getPos().first + slider.get()->getSize().first / 2.0, slider.get()->getPos().second - 10),
        std::make_pair(0.0, 400.0),
        400,
        10,
        true);
    
    BallSpeedBonus::LoadTexture(renderer, "SpeedUpBonus.png");
    SliderSizeBonus::LoadTexture(renderer, "ExtendedSliderBonus.png");
    StickyBonus::LoadTexture(renderer, "StickyBonus.png");
    Bonus::setStandartColor({0, 255, 255, 255});
    Field::loadFont(renderer, "ArcanoidFont.ttf");

    Field gameField(ball.get(), slider.get(), {WINDOW_WIDTH, WINDOW_HEIGHT});
    gameField.CreateRandomField(10, 7);
    float restartTime = 2.0f;
    while (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        restartTime = 0 ? restartTime <= 0 : restartTime - deltaTime;
       

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        SideToSlide sliderMove = SideToSlide::Stop;
        
        if (keystates[SDL_SCANCODE_LEFT]) {
            sliderMove = SideToSlide::Slider_Left;
        }
        else if (keystates[SDL_SCANCODE_RIGHT]) {
            sliderMove = SideToSlide::Slider_Right;
        }

        if (keystates[SDL_SCANCODE_SPACE]) {
            if (ball->getSticky()) {
                ball->launch(0);
            }
        }
        if (keystates[SDL_SCANCODE_R]) {
            if (restartTime <= 0) {
                gameField.reloadGame(10, 7);
                restartTime = 2.0f;
            }
            
        }

        gameField.Update(sliderMove, deltaTime);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        gameField.Draw(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}