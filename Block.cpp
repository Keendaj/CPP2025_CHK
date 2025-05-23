#include "Block.hpp"

std::vector<SDL_Color> Colors = {
	{128, 128, 128, 255}, // серый = неу€з
	{255, 0, 0, 255}, // красный = 1 хп
	{0, 255, 0, 255}, // зелЄный = 2 хп
	{0, 0, 255, 255}, // синий = 3 хп
	{255, 0, 255, 255}, // розовый = speedup
	{0, 255, 255, 255} // жЄлтый = бонус
};

std::vector<SDL_Color> BaseBlock::blockColors = Colors;


void BaseBlock::draw(SDL_Renderer* renderer) const {
	SDL_Color color = blockColors[maxHealth];
	SDL_SetRenderDrawColor(renderer,
		color.r * static_cast<float>(health) / maxHealth,
		color.g * static_cast<float>(health) / maxHealth,
		color.b * static_cast<float>(health) / maxHealth,
		color.a);

	SDL_Rect block = getRect();
	SDL_RenderFillRect(renderer, &block);
}

bool BaseBlock::getHit(Ball* ball) {
	health--;

	if (health <= 0) {
		isActive = false;
		return true;
	}

	return false;
}

SDL_Rect BaseBlock::getRect() const {
	return SDL_Rect(
		{static_cast<int>(pos.first),
		 static_cast<int>(pos.second),
		 static_cast<int>(size.first),
		 static_cast<int>(size.second),
		}
	);
}

void BaseBlock::setBlockColors(std::vector<SDL_Color> bC) {
	if (bC.size() >= 6) {
		blockColors = bC;
	}
	else {
		SDL_Log("Too few colours for the palette");
	}
}

void SpeedUpBlock::draw(SDL_Renderer* renderer) const {
	SDL_Color color = blockColors[4];
	SDL_SetRenderDrawColor(renderer,
		color.r * static_cast<float>(health) / maxHealth,
		color.g * static_cast<float>(health) / maxHealth,
		color.b * static_cast<float>(health) / maxHealth,
		color.a);

	SDL_Rect block = getRect();
	SDL_RenderFillRect(renderer, &block);
}

bool SpeedUpBlock::getHit(Ball* ball) {
	ball->setSpeed(ball->getSpeed() + speed_up);

	health--;

	if (health <= 0) {
		isActive = false;
		return true;
	}

	return false;
}

void InvincibleBlock::draw(SDL_Renderer* renderer) const {
	SDL_Color color = blockColors[0];
	SDL_SetRenderDrawColor(renderer, color.r , color.g, color.b, color.a);

	SDL_Rect block = getRect();
	SDL_RenderFillRect(renderer, &block);
}

bool InvincibleBlock::getHit(Ball* ball) {
	return false;
}

void BonusBlock::draw(SDL_Renderer* renderer) const {
	if (isActive) {
		SDL_Color color = blockColors[5];
		SDL_SetRenderDrawColor(renderer,
			color.r * static_cast<float>(health) / maxHealth,
			color.g * static_cast<float>(health) / maxHealth,
			color.b * static_cast<float>(health) / maxHealth,
			color.a);

		SDL_Rect block = getRect();
		SDL_RenderFillRect(renderer, &block);
	}
}

bool BonusBlock::getHit(Ball* ball) {
	return BaseBlock::getHit(ball);
}

bool MovingBlock::getHit(Ball* ball) {
	return BaseBlock::getHit(ball);
}

void MovingBlock::update(float deltaTime) {
	pos.first += direction * speed * deltaTime;
	if (pos.first >= rightBorder - size.first) {
		pos.first = rightBorder - size.first;
		direction *= -1;
	}
	if (pos.first <= leftBorder) {
		pos.first = leftBorder;
		direction *= -1;
	}
}