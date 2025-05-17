#include "Field.hpp"
#include <algorithm>
#include <cmath>

enum CollisionType {
    NO_COLLISION,
    VERTICAL,
    HORIZONTAL,
    CORNER
};

Field::Field(Ball* ball, Slider* slider, int windowWidth, int windowHeight) 
    : ball(ball), slider(slider), windowSize({ windowWidth, windowHeight }) {}

Field::Field(Ball* ball, Slider* slider, std::pair<int, int> windowSize) 
    : ball(ball), slider(slider), windowSize(windowSize) {}

Field::~Field() {
    delete ball;
    delete slider;
}

void Field::Update(float deltaTime)
{
    if (isGameOver()) return;

    auto oldBallPos = ball->getPos();
    ball->updateBall(deltaTime);
    auto newBallPos = ball->getPos();

    SDL_Rect sliderRect = slider->getHitboxRect();
    if (checkBallCollisionWithRect(&sliderRect)) {
        if (isBallSticky) {
            float relativeX = oldBallPos.first - slider->getPos().first;
            ball->setPos(slider->getPos().first + relativeX, slider->getPos().second - ball->getSize());
            ball->setVelocity(0, 0);
        } else {
 
            float hitPoint = (oldBallPos.first - slider->getPos().first) / slider->getHitboxWidth();
            float angle = (hitPoint - 0.5f) * 60.0f;
            float speed = ball->getSpeed();
            
            float radians = angle * M_PI / 180.0f;
            ball->setVelocity(speed * std::sin(radians), -speed * std::cos(radians));
            ball->setPos(newBallPos.first, slider->getPos().second - ball->getSize());
        }
    }
}

void Field::Draw(SDL_Renderer* renderer)
{
    if (hasSafetyNet) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_Rect netRect = {0, 590, 800, 5};
        SDL_RenderFillRect(renderer, &netRect);
    }

}

void Field::handleBallLost() {
    health--;
    if (!isGameOver()) {
        ball->setPos(400, 550);
        ball->setVelocity(0, -300);
        isBallSticky = false;
    }
}

void Field::InitializeBlocks(int rows, int cols)
{
}

bool Field::checkBallCollisionWithRect(SDL_Rect* rect)
{
    int ballX = ball->getPos().first;
    int ballY = ball->getPos().second;
    
    int closestX;
    int closestY;
    
    if (ballX < rect->x) {
        closestX = rect->x;
    } 
    else if (ballX > rect->x + rect->w) {
        closestX = rect->x + rect->w;
    } else {
        closestX = ballX;
    }
    
    if (ballY < rect->y) {
        closestY = rect->y;
    } 
    else if (ballY > rect->y + rect->h) {
        closestY = rect->y + rect->h;
    } 
    else {
        closestY = ballY;
    }
    
    int deltaX = ballX - closestX;
    int deltaY = ballY - closestY;
    int distanceSquared = deltaX * deltaX + deltaY * deltaY;
    
    int radiusSquared = ball->getSize() * ball->getSize();
    return distanceSquared <= radiusSquared;
}