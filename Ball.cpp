#include "Ball.hpp"
#include <cmath>

void Ball::drawBall(SDL_Renderer* renderer) {
    float centerX = pos.first;
    float centerY = pos.second;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (float y = -radius; y <= radius; y++) {
        float dx = static_cast<float>(sqrt(radius * radius - y * y));
        float x1 = centerX - dx;
        float x2 = centerX + dx;

        SDL_RenderDrawLine(renderer, x1, centerY + y, x2, centerY + y);
    }
}

void Ball::updateBall(float delta_time) {
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