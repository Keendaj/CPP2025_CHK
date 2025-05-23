#include "Bonus.hpp"
#include "Field.hpp"

SDL_Color Bonus::standartColor = { 255, 255, 255, 255 };
float Bonus::fall_speed = 50.0f;
float Bonus::maxEndtime = 30.0f;
float Bonus::size = 16;
int Bonus::padding = 15;

SDL_Texture* SliderSizeBonus::texture = nullptr;
float SliderSizeBonus::multiplier = 1.1f;

SDL_Texture* BallSpeedBonus::texture = nullptr;
float BallSpeedBonus::multiplier = 1.1f;

SDL_Texture* StickyBonus::texture = nullptr;

SDL_Texture* MovingBlockBonus::texture = nullptr;


void DrawTimerCircle(SDL_Renderer* renderer,
	int centerX,
	int centerY,
	int radius,
	float progress) {
	const int segments = 64;
	float angleStep = 2.0f * M_PI / segments;
	int lastX = centerX;
	int lastY = centerY;

	for (int i = 0; i <= segments * progress; ++i) {
		float angle = i * angleStep - M_PI / 2.0f;
		int x = static_cast<int>(centerX + radius * cos(angle));
		int y = static_cast<int>(centerY + radius * sin(angle));

		if (i > 0) {
			SDL_RenderDrawLine(renderer, lastX, lastY, x, y);
		}
		lastX = x;
		lastY = y;
	}
}

SDL_Rect Bonus::getRect() const {
	return SDL_Rect{
		static_cast<int>(pos.first),
		static_cast<int>(pos.second),
		static_cast<int>(size * 2),
		static_cast<int>(size * 2)
	};
}

void Bonus::Update(float deltaTime) {
	if (isActive) {
		if (curEndtime <= 0) {
			return;
		}
		else {
			curEndtime -= deltaTime;
		}
	}

	pos.second += fall_speed * deltaTime;
	if (pos.second <= 0) {
		isDropped = false;
	}
}


void SliderSizeBonus::doBonus(Ball* ball, Slider* slider, Field* field) {
	slider->setSizeMultiplayer(multiplier);
}

void SliderSizeBonus::removeBonus(Ball* ball, Slider* slider, Field* field) {
	slider->setSizeMultiplayer(1.0);
}

void SliderSizeBonus::LoadTexture(SDL_Renderer* renderer, const std::string& path) {
	texture = IMG_LoadTexture(renderer, path.c_str());
	if (!texture) {
		SDL_Log("Failed to load SliderSizeBonus texture: %s", IMG_GetError());
	}
}

void SliderSizeBonus::DestroyTexture() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void SliderSizeBonus::Draw(SDL_Renderer* renderer, int curBonusNumber) {
	SDL_Rect bonusRect = getRect();
	if (isDropped) {
		if (texture) {
			SDL_RenderCopy(renderer, texture, nullptr, &bonusRect);
		}
		else {

			SDL_SetRenderDrawColor(renderer,
				standartColor.r, 
				standartColor.g,
				standartColor.b,
				standartColor.a);

			SDL_RenderFillRect(renderer, &bonusRect);
		}
	}
	else if (isActive) {
		int centerX = padding + size;
		int centerY = padding + size + size * 2.6 * curBonusNumber;

		if (texture) {
			SDL_Rect iconRect = { padding + size / 2,
				padding + size / 2 + size * 2.6 * curBonusNumber ,
				size,
				size };
			SDL_RenderCopy(renderer, texture, nullptr, &iconRect);
		}

		float progress = curEndtime / maxEndtime;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		DrawTimerCircle(renderer, centerX, centerY, size + 2, progress);
	}
}



void BallSpeedBonus::LoadTexture(SDL_Renderer* renderer, const std::string& path) {
	texture = IMG_LoadTexture(renderer, path.c_str());
	if (!texture) {
		SDL_Log("Failed to load BallSpeedBonus texture: %s", IMG_GetError());
	}
}

void BallSpeedBonus::DestroyTexture() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}


void BallSpeedBonus::doBonus(Ball* ball, Slider* slider, Field* field) {
	ball->setSpeedMultiplier(multiplier);
}

void BallSpeedBonus::removeBonus(Ball* ball, Slider* slider, Field* field) {
	ball->setSpeedMultiplier(1.0);
}


void BallSpeedBonus::Draw(SDL_Renderer* renderer, int curBonusNumber) {
	SDL_Rect bonusRect = getRect();
	if (isDropped) {
		if (texture) {
			SDL_RenderCopy(renderer, texture, nullptr, &bonusRect);
		}
		else {

			SDL_SetRenderDrawColor(renderer,
				standartColor.r,
				standartColor.g,
				standartColor.b,
				standartColor.a);

			SDL_RenderFillRect(renderer, &bonusRect);
		}
	}
	else if (isActive) {
		int centerX = padding + size;
		int centerY = padding + size + size * 2.6 * curBonusNumber;

		if (texture) {
			SDL_Rect iconRect = { padding + size / 2,
				padding + size / 2 + size * 2.6 * curBonusNumber ,
				size,
				size };
			SDL_RenderCopy(renderer, texture, nullptr, &iconRect);
		}

		float progress = curEndtime / maxEndtime;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		DrawTimerCircle(renderer, centerX, centerY, size, progress);
	}
}


void StickyBonus::LoadTexture(SDL_Renderer* renderer, const std::string& path) {
	texture = IMG_LoadTexture(renderer, path.c_str());
	if (!texture) {
		SDL_Log("Failed to load StickyBonus texture: %s", IMG_GetError());
	}
}

void StickyBonus::DestroyTexture() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

void StickyBonus::Draw(SDL_Renderer* renderer, int curBonusNumber) {
	SDL_Rect bonusRect = getRect();


	if (isDropped) {
		if (texture) {
			SDL_RenderCopy(renderer, texture, nullptr, &bonusRect);
		}
		else {

			SDL_SetRenderDrawColor(renderer,
				standartColor.r,
				standartColor.g, 
				standartColor.b,
				standartColor.a);

			SDL_RenderFillRect(renderer, &bonusRect);
		}
	}
	else if (isActive) {
		int centerX = padding + size;
		int centerY = padding + size + size * 2.6 * curBonusNumber;
		SDL_Rect iconRect = { padding + size / 2,
			padding + size / 2 + size * 2.6 * curBonusNumber,
			size,
			size };
		if (texture) {
			SDL_RenderCopy(renderer, texture, nullptr, &iconRect);
		}
		else {
			SDL_SetRenderDrawColor(renderer,
				standartColor.r, 
				standartColor.g, 
				standartColor.b,
				standartColor.a);
			SDL_RenderFillRect(renderer, &iconRect);
		}
	}
}


void MovingBlockBonus::LoadTexture(SDL_Renderer* renderer, const std::string& path) {
	texture = IMG_LoadTexture(renderer, path.c_str());
	if (!texture) {
		SDL_Log("Failed to load MovingBlockBonus texture: %s", IMG_GetError());
	}
}

void MovingBlockBonus::DestroyTexture() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}


void MovingBlockBonus::Draw(SDL_Renderer* renderer, int curBonusNumber) {
	SDL_Rect bonusRect = getRect();
	if (isDropped) {
		if (texture) {
			SDL_RenderCopy(renderer, texture, nullptr, &bonusRect);
		}
		else {

			SDL_SetRenderDrawColor(renderer,
				standartColor.r, 
				standartColor.g,
				standartColor.b,
				standartColor.a);

			SDL_RenderFillRect(renderer, &bonusRect);
		}
	}
}


void MovingBlockBonus::doBonus(Ball* ball, Slider* slider, Field* field) {
	MovingBlock* bk = new MovingBlock(blockPos,
		blockSize,
		blockHealth,
		blockSpeed,
		leftBorder,
		rightBorder);
	field->addBlock(bk);
}