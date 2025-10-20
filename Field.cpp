#include "Field.hpp"

SDL_Texture* Field::healthIcon = nullptr;
int Field::healthSize = 32;

TTF_Font* Field::font = nullptr;
int Field::fontSize = 16;

Field::Field(Ball* ball, 
    Slider* slider, 
    int windowWidth, 
    int windowHeight) 
    : ball(ball),
    slider(slider),
    windowSize({ windowWidth, windowHeight }),
    isOneTimeNet(false)
    { ball->setWindowSize(windowWidth, windowHeight); 
    slider->setWindowSize(windowWidth, windowHeight); }

Field::Field(Ball* ball, 
    Slider* slider,
    std::pair<int, int> windowSize) 
    : ball(ball),
    slider(slider),
    windowSize(windowSize),
    isOneTimeNet(false)
    { ball->setWindowSize(windowSize.first, windowSize.second);
    slider->setWindowSize(windowSize.first, windowSize.second); }

Field::~Field() {
    for (auto i : droppedBonuses) {
        if (i) {
            delete i;
        }
        
    }
    for (auto i : blocks) {
        if (i) {
            if (typeid(*i) == typeid(BonusBlock)) {
                BonusBlock* bb = dynamic_cast<BonusBlock*>(i);
                if (bb->getBonus()) {
                    delete bb->getBonus();
                }
            }
            delete i;
        }
    }

    destroyFont();
    destroyHealtIcon();
}

void Field::update(SideToSlide sliderMove, float deltaTime)
{
    if (isGameOver()) return;

    for (auto i : droppedBonuses) {
        if (i && (i->getIsDropped() || i->getIsActive())) {
            i->update(deltaTime);
        }
    }

    ball->updateBall(deltaTime);
    auto BallPos = ball->getPos();

    slider->updateSlider(sliderMove, deltaTime);
    if (BallPos.second - ball->getSize() >= windowSize.second) {
        if (isOneTimeNet) {
            for (auto i : droppedBonuses) {
                if (typeid(*i) == typeid(OneTimeNetBonus)) {
                    auto bonus = dynamic_cast<OneTimeNetBonus*>(i);
                    bonus->removeBonus(ball, slider, this);
                    break;
                }
            }
            auto velocity = ball->getVelocity();
            ball->setVelocity(velocity.first, -1 * velocity.second);
        }
        else {
            handleBallLost();
        }
    }
    else {
        SDL_Rect sliderRect = slider->getRect();
        if (ball->checkCollisionWithRect(&sliderRect)) {
            for (auto i : droppedBonuses) {
                if (dynamic_cast<StickyBonus*>(i) && !i->getIsDropped() && !ball->getSticky()) {
                    ball->setSticky(true);
                    i->setIsActive(false);
                    break;
                }
            }
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
        if (dynamic_cast<MovingBlock*>(i)) {
            dynamic_cast<MovingBlock*>(i) -> update(deltaTime);
        }
        if (!i || !i->getIsActive()) continue;
        SDL_Rect blockRect = i->getRect();
        if (ball->checkCollisionWithRect(&blockRect)) {
            if (!dynamic_cast<InvincibleBlock*>(i)) {
                score++;
            }
            if (i->getHit(ball)) {
                if (typeid(*i) == typeid(BonusBlock)) {
                    BonusBlock* bonus_block = dynamic_cast<BonusBlock*>(i);
                    Bonus* bs = bonus_block->getBonus();
                    if (bs) {
                        bs->setIsDropped(true);
                        bs->setIsActive(false);
                        bs->resetEndtime();

                        droppedBonuses.push_back(bs);
                    }
                    
                }
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

    for (auto i : droppedBonuses) {
        SDL_Rect bonusRect = i->getRect();
        SDL_Rect sliderRect = slider->getRect();
        if (i->getIsDropped() && SDL_HasIntersection(&bonusRect, &sliderRect)) {
            bool alreadyActive = false;

            for (auto j : droppedBonuses) {
                if (j == i) continue;
                if (j->getIsActive() && typeid(*j) == typeid(*i)) {
                    j->resetEndtime();
                    alreadyActive = true;
                    break;
                }
            }

            if (!alreadyActive) {
                i->doBonus(ball, slider, this);
                i->setIsActive(true);
            }
            i->setIsDropped(false);
        }
    }

    CleanDestroyedBlocks();
    CleanDestroyedBonuses();
}

void Field::draw(SDL_Renderer* renderer)
{
    slider->drawSlider(renderer);
    ball->drawBall(renderer);

    for (auto i : blocks) {
        if (i && i->getIsActive()) {
            i->draw(renderer);
        }
    }
    int curBonusNumber = 0;
    for (auto i : droppedBonuses) {
        if (i && (i && (i->getIsDropped() || i->getIsActive()))) {
            i->draw(renderer, curBonusNumber);
            curBonusNumber++;
        }
    }
    for (int i = 0; i < health; ++i) {
        SDL_Rect dst = { windowSize.first - healthSize - 10 - i * (healthSize+10), 10, healthSize, healthSize };
        if (healthIcon) {
            SDL_RenderCopy(renderer, healthIcon, NULL, &dst);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &dst);
        }
    }
    if (font) {
        SDL_Color color = { 255, 255, 255, 255 };

        std::string score_text = "Score: " + std::to_string(score);

        SDL_Surface* score_surface = TTF_RenderText_Blended(font, score_text.c_str(), color);
        SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);

        SDL_Rect score_rect = { windowSize.first - score_surface->w - 10, 20 + healthSize, score_surface->w, score_surface->h };
        SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);

        SDL_FreeSurface(score_surface);
        SDL_DestroyTexture(score_texture);
    }

}


void Field::createRandomField(int cols, int rows) {
    int centerX = windowSize.first / 2;

    float fieldWidth = cols * (BLOCK_WIDTH + BLOCK_PADDING) - BLOCK_PADDING;
    float startX = centerX - fieldWidth / 2.0f;
    float startY = BLOCKS_TOP_MARGIN;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 10);
    std::uniform_int_distribution<> bonusDist(3, 3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float posX = startX + j * (BLOCK_WIDTH + BLOCK_PADDING);
            float posY = startY + i * (BLOCK_HEIGHT + BLOCK_PADDING);

            BaseBlock* block = nullptr;
            int blockType = dist(gen);
            if (blockType >= 9) {
                Bonus* bonus = nullptr;
                switch (bonusDist(gen))
                {
                    case 0:
                        bonus = new BallSpeedBonus({posX, posY});
                        break;
                    case 1:
                        bonus = new SliderSizeBonus({ posX, posY });
                        break;
                    case 2:
                        bonus = new StickyBonus({ posX, posY });
                        break;
                    case 3:
                        bonus = new MovingBlockBonus({ posX, posY },
                            100,
                            3,
                            100,
                            windowSize.first - 100,
                            {150, windowSize.second / 2.0 },
                            { BLOCK_WIDTH * 2, BLOCK_HEIGHT });
                    case 4:
                        bonus = new OneTimeNetBonus({ posX, posY });
                    default:
                        break;
                }
                block = new BonusBlock({ posX, posY }, { BLOCK_WIDTH, BLOCK_HEIGHT }, 1, bonus);
            }
            else if (blockType >= 8) {
                block = new InvincibleBlock({ posX, posY }, { BLOCK_WIDTH, BLOCK_HEIGHT });
            }
            else if (blockType >= 7) {
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
        [this](BaseBlock* block) {
            if (!block->getIsActive()) {
                if (dynamic_cast<MovingBlock*>(block)) {
                    for (auto i : droppedBonuses) {
                        if (dynamic_cast<MovingBlockBonus*>(i)) {
                            i->setIsActive(false);
                            break;
                        }
                    }
                }
                delete block;
                return true;
            }
            return false;
        }),
        blocks.end());
}

void Field::CleanDestroyedBonuses() {
    for (auto& bonus : droppedBonuses) {
        if (bonus->getIsActive() && bonus->getEndtime() <= 0.0f) {
            bonus->removeBonus(ball, slider, this);
        }
    }

    droppedBonuses.erase(std::remove_if(droppedBonuses.begin(), droppedBonuses.end(),
        [](Bonus* bonus) {
            if (!bonus->getIsDropped() && !bonus->getIsActive()) {
                delete bonus;
                return true;
            }
            return false;
        }), droppedBonuses.end());
}

void Field::handleBallLost() {
    health--;
    if (!isGameOver()) {
        ball->setPos(slider->getPos().first + slider->getSize().first / 2.0, slider->getPos().second - 10);
        ball->setSticky(true);
    }
}

void Field::loadHealthIcon(SDL_Renderer* renderer, const std::string& path) {
    healthIcon = IMG_LoadTexture(renderer, path.c_str());
}

void Field::loadFont(SDL_Renderer* renderer, const std::string& path) {
   font = TTF_OpenFont(path.c_str(), fontSize);
   if (!font) {
       SDL_Log("Failed to load font: %s", TTF_GetError());
   }
}

void Field::reloadGame(int cols, int rows) {
    health = 3;
    score = 0;
    droppedBonuses.clear();
    blocks.clear();

    slider->setPos(std::make_pair(windowSize.first / 2.0f - 50, windowSize.second - 50));

    ball->setSticky(true);
    ball->setPos(std::make_pair(slider->getPos().first + slider->getSize().first / 2.0, slider->getPos().second - 10));
    ball->setSpeed(400);
    ball->setVelocity(0, 400);

    createRandomField(cols, rows);
}

void Field::addBlock(BaseBlock* bk) {
    blocks.push_back(bk);
}

void Field::destroyFont() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

void Field::destroyHealtIcon() {
    if (healthIcon) {
        SDL_DestroyTexture(healthIcon);
        healthIcon = nullptr;
    }
}