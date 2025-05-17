#include "Ball.hpp"
#include <cmath>
#include <algorithm>
#define _USE_MATH_DEFINES

Ball::Ball(float x, float y, float x_velocity, float y_velocity, float baseSpeed, float radius, bool isSticky)
    : pos({ x, y }), velocity({ x_velocity, y_velocity }), baseSpeed(baseSpeed), radius(radius), isSticky(isSticky)
{
    normalizeVelocity();
}

Ball::Ball(std::pair<float, float> pos, std::pair<float, float> velocity, float baseSpeed, float radius, bool isSticky)
    : pos(pos), velocity(velocity), baseSpeed(baseSpeed), radius(radius), isSticky(isSticky)
{
    normalizeVelocity();
}


void Ball::drawBall(SDL_Renderer* renderer) {
    int centerX = static_cast<int>(pos.first);
    int centerY = static_cast<int>(pos.second);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (int y = -radius; y <= radius; y++) {
        int dx = static_cast<int>(sqrt(radius * radius - y * y));
        int x1 = centerX - dx;
        int x2 = centerX + dx;
        int yDraw = centerY + y;

        SDL_RenderDrawLine(renderer, x1, yDraw, x2, yDraw);
    }
}


void Ball::updateBall(float delta_time) {
    if (isSticky) return;

    pos.first += velocity.first * delta_time;
    pos.second += velocity.second * delta_time;

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

void Ball::setVelocity(float x_velocity, float y_velocity) {
    velocity = {x_velocity, y_velocity};
    normalizeVelocity();
}

void Ball::setVelocity(std::pair<float, float> new_velocity) {
    velocity = new_velocity;
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
        velocity.first = (velocity.first / speed) * baseSpeed;
        velocity.second = (velocity.second / speed) * baseSpeed;
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