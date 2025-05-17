#include "Field.hpp"
#include <algorithm>
#include <cmath>

Field::Field(Ball* ball, Slider* slider, int windowWidth, int windowHeight) 
    : ball(ball), slider(slider), windowSize({ windowWidth, windowHeight })
    { ball->setWindowSize(windowWidth, windowHeight); }

Field::Field(Ball* ball, Slider* slider, std::pair<int, int> windowSize) 
    : ball(ball), slider(slider), windowSize(windowSize) {}

Field::~Field() {
    delete ball;
    delete slider;
}

void Field::Update(SideToSlide sliderMove, float deltaTime)
{
    if (isGameOver()) return;

    ball->updateBall(deltaTime);
    auto BallPos = ball->getPos();

    slider->updateSlider(sliderMove, deltaTime);

    SDL_Rect sliderRect = slider->getRect();
    if (ball->checkCollisionWithRect(&sliderRect)) {
        if (ball->getSticky()) {
            ball->setPos(slider->getPos().first + slider->getSize().first / 2.0, slider->getPos().second - ball->getSize());
            ball->setVelocity(0, 0);
        } 
        else {
            float hitPoint = (BallPos.first - slider->getPos().first) / slider->getSize().first;
            float angle = (hitPoint - 0.5f) * 60.0f;
            float speed = ball->getSpeed();
            
            float radians = angle * M_PI / 180.0f;
            ball->setVelocity(speed * std::sin(radians), -speed * std::cos(radians));
            ball->setPos(BallPos.first, slider->getPos().second - ball->getSize());
        }
    }
}

void Field::Draw(SDL_Renderer* renderer)
{
    slider->drawSlider(renderer);
    ball->drawBall(renderer);
}

void Field::handleBallLost() {
    health--;
    if (!isGameOver()) {
        ball->setPos(400, 550);
        ball->setVelocity(0, -300);
        ball->setSticky(false);
    }
}

void Field::InitializeBlocks(int rows, int cols)
{
}