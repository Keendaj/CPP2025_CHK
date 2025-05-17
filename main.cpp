#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Field.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
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

    Ball* ball = new Ball({WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 100}, 10);
    Slider* slider = new Slider({WINDOW_WIDTH / 2.0f - 50, WINDOW_HEIGHT - 50}, {100, 20}, 400.0f);
    Field gameField(ball, slider, {WINDOW_HEIGHT, WINDOW_WIDTH});
    gameField.InitializeBlocks(5, 10);

    while (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

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

        ball->updateBall(deltaTime);
        slider->updateSlider(renderer, sliderMove, deltaTime);
        gameField.Update(deltaTime);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        gameField.Draw(renderer);
        ball->drawBall(renderer);
        slider->drawSlider(renderer);

        SDL_RenderPresent(renderer);
    }

    delete ball;
    delete slider;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}