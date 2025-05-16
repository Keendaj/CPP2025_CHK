#include "GameBoard.hpp"
#include "GameConfig.hpp"
#include <algorithm>
#include <random>
#include <ctime>

GameBoard::GameBoard(int size, double bombChance, double recolorChance)
    : bombChance(bombChance), recolorChance(recolorChance), selectedTile({-1, -1}) {
    board.resize(size);
    for (auto& column : board) {
        column.resize(size);
    }
    std::srand(std::time(0));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::vector<int> possibleColors;
            for (int color = 1; color < Config::getColors().size(); ++color) {
                possibleColors.push_back(color);
            }

            if (j >= 2 && board[i][j-1] && board[i][j-2]) {
                if (board[i][j-1]->getColor() == board[i][j-2]->getColor()) {
                    auto it = std::find(possibleColors.begin(), possibleColors.end(), board[i][j-1]->getColor());
                    if (it != possibleColors.end()) {
                        possibleColors.erase(it);
                    }
                }
            }

            if (i >= 2 && board[i-1][j] && board[i-2][j]) {
                if (board[i-1][j]->getColor() == board[i-2][j]->getColor()) {
                    auto it = std::find(possibleColors.begin(), possibleColors.end(), board[i-1][j]->getColor());
                    if (it != possibleColors.end()) {
                        possibleColors.erase(it);
                    }
                }
            }

            int color;
            if (possibleColors.empty()) {
                color = std::rand() % (Config::getColors().size() - 1) + 1;
            } else {
                color = possibleColors[std::rand() % possibleColors.size()];
            }
            
            board[i][j] = std::make_unique<BasicTile>(color);
        }
    }
}

void GameBoard::update(double deltaTime) {
    const auto& animConfig = Config::getAnimation();
    
    for (auto& animation : animations) {
        if (animation.active) {
            animation.scale -= animConfig.scaleSpeed * deltaTime;
            animation.alpha -= animConfig.alphaSpeed * deltaTime;

            if (animation.scale <= 0 || animation.alpha <= 0) {
                animation.active = false;
                board[animation.pos.first][animation.pos.second] = nullptr;
            }
        }
    }

    animations.erase(
        std::remove_if(animations.begin(), animations.end(),
            [](const Animation& a) { return !a.active; }),
        animations.end()
    );
}

void GameBoard::render(SDL_Renderer* renderer, int screenWidth, int screenHeight) const {
    int tileWidth = screenWidth / board.size();
    int tileHeight = screenHeight / board[0].size();

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (board[i][j] && !isTileAnimating(i, j)) {
                SDL_Rect bounds = {
                    i * tileWidth,
                    j * tileHeight,
                    tileWidth,
                    tileHeight
                };
                board[i][j]->render(renderer, bounds);
            }
        }
    }

    for (const auto& animation : animations) {
        if (animation.active) {
            SDL_Rect bounds = {
                static_cast<int>(animation.pos.first * tileWidth + tileWidth * (1 - animation.scale) / 2),
                static_cast<int>(animation.pos.second * tileHeight + tileHeight * (1 - animation.scale) / 2),
                static_cast<int>(tileWidth * animation.scale),
                static_cast<int>(tileHeight * animation.scale)
            };
            
            if (board[animation.pos.first][animation.pos.second]) {
                board[animation.pos.first][animation.pos.second]->render(
                    renderer,
                    bounds,
                    static_cast<Uint8>(animation.alpha));
            }
        }
    }
}

void GameBoard::handleClick(int mouseX, int mouseY, int screenWidth, int screenHeight) {
    int tileWidth = screenWidth / board.size();
    int tileHeight = screenHeight / board[0].size();
    int i = mouseX / tileWidth;
    int j = mouseY / tileHeight;

    if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size()) return;

    if (selectedTile.first == -1) {
        if (board[i][j]) {
            selectedTile = {i, j};
            board[i][j]->setSelected(true);
        }
        return;
    }

    if ((std::abs(selectedTile.first - i) + std::abs(selectedTile.second - j)) == 1) {
        std::swap(board[selectedTile.first][selectedTile.second], board[i][j]);

        auto matches = checkMatches();
        if (matches.empty()) {
            std::swap(board[selectedTile.first][selectedTile.second], board[i][j]);
        } 
        else {
            createBonusTile({i, j});
        }
    }

    if (board[selectedTile.first][selectedTile.second]) {
        board[selectedTile.first][selectedTile.second]->setSelected(false);
    }
    selectedTile = {-1, -1};
}

void GameBoard::fillEmptySpaces(int width, int height) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = board[0].size() - 1; j >= 0; j--) {
            if (!board[i][j]) {
                int fallFrom = j - 1;
                while (fallFrom >= 0 && !board[i][fallFrom]) {
                    fallFrom--;
                }

                if (fallFrom >= 0) {
                    board[i][j] = std::move(board[i][fallFrom]);
                } 
                else {
                    board[i][j] = std::make_unique<BasicTile>(std::rand() % 5 + 1);
                }
            }
        }
    }
}

std::vector<std::pair<int, int>> GameBoard::checkLine(std::pair<int, int> coords, bool vertical) const {
    std::vector<std::pair<int, int>> matches;
    int x = coords.first, y = coords.second;
    
    if (!board[x][y]) return matches;
    int color = board[x][y]->getColor();

    if (vertical) {
        int top = y, bottom = y;
        while (top > 0 && board[x][top-1] && board[x][top-1]->getColor() == color) top--;
        while (bottom < board[0].size()-1 && board[x][bottom+1] && board[x][bottom+1]->getColor() == color) bottom++;

        if (bottom - top >= 2) {
            for (int i = top; i <= bottom; i++) {
                matches.emplace_back(x, i);
            }
        }
    } 
    else {
        int left = x, right = x;
        while (left > 0 && board[left-1][y] && board[left-1][y]->getColor() == color) left--;
        while (right < board.size()-1 && board[right+1][y] && board[right+1][y]->getColor() == color) right++;

        if (right - left >= 2) {
            for (int i = left; i <= right; i++) {
                matches.emplace_back(i, y);
            }
        }
    }

    return matches;
}

std::set<std::pair<int, int>> GameBoard::checkMatches() const {
    std::set<std::pair<int, int>> allMatches;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (!board[i][j]) continue;

            auto vertMatches = checkLine({i, j}, true);
            auto horzMatches = checkLine({i, j}, false);

            allMatches.insert(vertMatches.begin(), vertMatches.end());
            allMatches.insert(horzMatches.begin(), horzMatches.end());
        }
    }

    return allMatches;
}

void GameBoard::clearMatchedTiles(const std::set<std::pair<int, int>>& matches) {
    std::vector<std::pair<int, int>> bombTiles;
    std::vector<std::pair<int, int>> recolorTiles;

    for (const auto& pos : matches) {
        if (auto* bonusTile = dynamic_cast<BonusTile*>(board[pos.first][pos.second].get())) {
            if (dynamic_cast<BombTile*>(bonusTile)) {
                bombTiles.push_back(pos);
            } 
            else if (dynamic_cast<RecolorTile*>(bonusTile)) {
                recolorTiles.push_back(pos);
            }
        }
        animations.push_back({pos, 1.0, 255.0, true});
    }

    for (const auto& pos : bombTiles) {
        handleBombActivation(pos.first, pos.second);
        animations.push_back({pos, 1.0, 255.0, true});
    }
    for (const auto& pos : recolorTiles) {
        handleRecolorActivation(pos.first, pos.second);
        animations.push_back({pos, 1.0, 255.0, true});
    }
}

void GameBoard::createBonusTile(std::pair<int, int> pos) {
    double rand = static_cast<double>(std::rand()) / RAND_MAX;
    
    if (rand < bombChance) {
        int x = pos.first + std::rand() % 7 - 3;
        int y = pos.second + std::rand() % 7 - 3;
        
        if (x >= 0 && x < board.size() && y >= 0 && y < board[0].size() && board[x][y]) {
            int color = board[x][y]->getColor();
            board[x][y] = std::make_unique<BombTile>(color);
        }
    } 
    else if (rand < bombChance + recolorChance) {
        int x = pos.first + std::rand() % 7 - 3;
        int y = pos.second + std::rand() % 7 - 3;
        
        if (x >= 0 && x < board.size() && y >= 0 && y < board[0].size() && board[x][y]) {
            int color = board[x][y]->getColor();
            board[x][y] = std::make_unique<RecolorTile>(color);
        }
    }
}

void GameBoard::handleBombActivation(int x, int y) {
    std::vector<std::pair<int, int>> targets;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (i == x && j == y) continue;
            if (board[i][j] && !isTileAnimating(i, j)) {
                targets.emplace_back(i, j);
            }
        }
    }

    std::random_shuffle(targets.begin(), targets.end());
    int targetsToMark = std::min(4, static_cast<int>(targets.size()));

    for (int i = 0; i < targetsToMark; i++) {
        int tx = targets[i].first;
        int ty = targets[i].second;
        if (board[tx][ty] && !isTileAnimating(tx, ty)) {
            animations.push_back({targets[i], 1.0, 255.0, true});
        }
    }
}

void GameBoard::handleRecolorActivation(int x, int y) {
    if (!board[x][y]) return;
    int targetColor = board[x][y]->getColor();

    std::vector<std::pair<int, int>> possibleTargets;
    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -3; dy <= 3; dy++) {
            if (dx * dx + dy * dy <= 1) continue;

            int newX = x + dx;
            int newY = y + dy;

            if (newX >= 0 && newX < board.size() && newY >= 0 && newY < board[0].size() &&
                board[newX][newY] && !isTileAnimating(newX, newY) &&
                board[newX][newY]->getColor() != targetColor) {
                possibleTargets.emplace_back(newX, newY);
            }
        }
    }

    int targetsToChange = std::min(2, static_cast<int>(possibleTargets.size()));
    std::random_shuffle(possibleTargets.begin(), possibleTargets.end());

    for (int i = 0; i < targetsToChange; i++) {
        int tx = possibleTargets[i].first;
        int ty = possibleTargets[i].second;
        board[tx][ty]->setColor(targetColor);
    }
}

bool GameBoard::isTileAnimating(int x, int y) const {
    return std::any_of(animations.begin(), animations.end(),
        [x, y](const Animation& animation) {
            return animation.pos.first == x && animation.pos.second == y && animation.active;
        });
} 