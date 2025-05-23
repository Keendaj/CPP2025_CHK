#include "Ball.hpp"
#include <cmath>
#include <algorithm>
#define _USE_MATH_DEFINES

Ball::Ball(float x,
    float y,
    float xVelocity, 
    float yVelocity, 
    float baseSpeed,
    float radius,
    bool isSticky,
    SDL_Color color)
    : pos({ x, y }), 
    velocity({ xVelocity, yVelocity }),
    baseSpeed(baseSpeed),
    radius(radius), 
    isSticky(isSticky),
    speedMultiplaer(1.0),
    color(color)
{
    normalizeVelocity();
}

Ball::Ball(std::pair<float, float> pos,
    std::pair<float, float> velocity, 
    float baseSpeed,
    float radius, 
    bool isSticky,
    SDL_Color color)
    : pos(pos), 
    velocity(velocity),
    baseSpeed(baseSpeed),
    radius(radius),
    isSticky(isSticky),
    speedMultiplaer(1.0),
    color(color)
{
    normalizeVelocity();
}


void Ball::drawBall(SDL_Renderer* renderer) {
    int centerX = static_cast<int>(pos.first);
    int centerY = static_cast<int>(pos.second);

    SDL_SetRenderDrawColor(renderer, color.r, color.b, color.g, color.a);

    for (int y = -radius; y <= radius; y++) {
        int dx = static_cast<int>(sqrt(radius * radius - y * y));
        int x1 = centerX - dx;
        int x2 = centerX + dx;
        int yDraw = centerY + y;

        SDL_RenderDrawLine(renderer, x1, yDraw, x2, yDraw);
    }
}


void Ball::updateBall(float deltaTime) {
    if (isSticky) return;

    pos.first += velocity.first * deltaTime;
    pos.second += velocity.second * deltaTime;

    if (pos.first <= radius) {
        pos.first = radius;
        velocity.first = std::abs(velocity.first);
    } else if (pos.first >= windowWidth - radius) {
        pos.first = windowWidth - radius;
        velocity.first = -std::abs(velocity.first);
    }
    if (pos.second <= radius) {
        pos.second = radius;
        velocity.second = std::abs(velocity.second);
    }
}

void Ball::setVelocity(float xVelocity, float yVelocity) {
    velocity = {xVelocity, yVelocity};
    normalizeVelocity();
}

void Ball::setVelocity(std::pair<float, float> newVelocity) {
    velocity = newVelocity;
    normalizeVelocity();
}

void Ball::launch(float angle) {
    isSticky = false;
    float radians = angle * M_PI / 180.0f;
    velocity.first = baseSpeed * std::sin(radians);
    velocity.second = -baseSpeed * std::cos(radians);
    normalizeVelocity();
}

void Ball::normalizeVelocity() {
    float speed = std::sqrt(velocity.first * velocity.first + velocity.second * velocity.second);
    if (speed > 0) {
        velocity.first = (velocity.first / speed) * getSpeed();
        velocity.second = (velocity.second / speed) * getSpeed();
    }
}

bool Ball::checkCollisionWithRect(SDL_Rect* rect)
{
    float ballX = pos.first;
    float ballY = pos.second;

    float closestX;
    float closestY;

    if (ballX < rect->x) {
        closestX = rect->x;
    }
    else if (ballX > rect->x + rect->w) {
        closestX = rect->x + rect->w;
    }
    else {
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

    int radiusSquared = radius * radius;
    return distanceSquared <= radiusSquared;
}