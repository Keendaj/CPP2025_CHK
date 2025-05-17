#include "Field.hpp"
#include <algorithm>
#include <random>
#include <cmath>

Field::Field(Ball* ball, Slider* slider, int windowWidth, int windowHeight) 
    : ball(ball), slider(slider), windowSize({ windowWidth, windowHeight })
    { ball->setWindowSize(windowWidth, windowHeight); slider->setWindowSize(windowWidth, windowHeight); }

Field::Field(Ball* ball, Slider* slider, std::pair<int, int> windowSize) 
    : ball(ball), slider(slider), windowSize(windowSize)
    { ball->setWindowSize(windowSize.first, windowSize.second); slider->setWindowSize(windowSize.first, windowSize.second); }

void Field::Update(SideToSlide sliderMove, float deltaTime)
{
    if (isGameOver()) return;

    ball->updateBall(deltaTime);
    auto BallPos = ball->getPos();

    slider->updateSlider(sliderMove, deltaTime);
    if (BallPos.second - ball->getSize() >= windowSize.second) {
        handleBallLost();
    }
    else {
        SDL_Rect sliderRect = slider->getRect();
        if (ball->checkCollisionWithRect(&sliderRect)) {
            if (ball->getSticky()) {
                ball->setPos(slider->getPos().first + slider->getSize().first / 2.0, slider->getPos().second - ball->getSize() / 2);
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

    for (auto i : blocks) {
        if (!i || !i->getIsActive()) continue;
        SDL_Rect blockRect = i->getRect();
        if (ball->checkCollisionWithRect(&blockRect)) {
            if (i->getHit(ball)) {
                i->setIsActive(false);
            }

            float ballLeft = BallPos.first - ball->getSize();
            float ballRight = BallPos.first + ball->getSize();
            float ballTop = BallPos.second - ball->getSize();
            float ballBottom = BallPos.second + ball->getSize();

            float blockLeft = blockRect.x;
            float blockRight = blockRect.x + blockRect.w;
            float blockTop = blockRect.y;
            float blockBottom = blockRect.y + blockRect.h;

            float rightHit = std::abs(ballRight - blockLeft);
            float leftHit = std::abs(blockRight - ballLeft);
            float bottomHit = std::abs(ballBottom - blockTop);
            float topHit = std::abs(blockBottom - ballTop);

            float minOverlap = std::min({ rightHit, leftHit, topHit, bottomHit });

            auto velocity = ball->getVelocity();
            if (minOverlap == bottomHit) {
                ball->setVelocity(velocity.first, -std::abs(velocity.second));
                ball->setPos(BallPos.first, blockTop - ball->getSize());
            }
            else if (minOverlap == topHit) {
                ball->setVelocity(velocity.first, std::abs(velocity.second));
                ball->setPos(BallPos.first, blockBottom + ball->getSize());
            }
            else if (minOverlap == rightHit) {
                ball->setVelocity(-std::abs(velocity.first), velocity.second);
                ball->setPos(blockLeft - ball->getSize(), BallPos.second);
            }
            else if (minOverlap == leftHit) {
                ball->setVelocity(std::abs(velocity.first), velocity.second);
                ball->setPos(blockRight + ball->getSize(), BallPos.second);
            }
        }
        
    }

    CleanDestroyedBlocks();
}

void Field::Draw(SDL_Renderer* renderer)
{
    slider->drawSlider(renderer);
    ball->drawBall(renderer);

    for (auto i : blocks) {
        if (i && i->getIsActive()) {
            i->Draw(renderer);
        }
    }
}


void Field::CreateRandomField(int cols, int rows) {
    int centerX = windowSize.first / 2;

    float fieldWidth = cols * (BLOCK_WIDTH + BLOCK_PADDING) - BLOCK_PADDING;
    float startX = centerX - fieldWidth / 2.0f;
    float startY = BLOCKS_TOP_MARGIN;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 2);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float posX = startX + j * (BLOCK_WIDTH + BLOCK_PADDING);
            float posY = startY + i * (BLOCK_HEIGHT + BLOCK_PADDING);

            BaseBlock* block = nullptr;
            int blockType = dist(gen);

            if (blockType == 2) {
                block = new InvincibleBlock({ posX, posY }, { BLOCK_WIDTH, BLOCK_HEIGHT });
            }
            else if (blockType == 1) {
                block = new SpeedUpBlock({ posX, posY }, { BLOCK_WIDTH, BLOCK_HEIGHT }, 3, 40);
            }
            else {
                block = new BaseBlock({ posX, posY }, { BLOCK_WIDTH, BLOCK_HEIGHT }, (j + i) % 3 + 1);
            }

            blocks.push_back(block);
        }
    }
}

void Field::CleanDestroyedBlocks() {
    blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
        [](BaseBlock* block) {
            if (!block->getIsActive()) {
                delete block;
                return true;
            }
            return false;
        }),
        blocks.end());
}
void Field::handleBallLost() {
    health--;
    if (!isGameOver()) {
        ball->setPos(slider->getPos().first + slider->getSize().first / 2.0, slider->getPos().second - 10);
        ball->setSticky(true);
    }
}
