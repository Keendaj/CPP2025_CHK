#pragma once
#include <SDL2/SDL.h>
#include "Ball.hpp"
#include <utility>
#include <vector>



class BaseBlock
{
	public:
		BaseBlock(std::pair<float, float> pos, std::pair<float, float> size, int health)
			: pos(pos), size(size), health(health), maxHealth(health), isActive(true) { }

		virtual void Draw(SDL_Renderer* renderer) const;
		virtual bool getHit(Ball* ball); // true if destroyed

		SDL_Rect getRect() const;

		std::pair<float, float> getPos() const { return pos; }
		std::pair<float, float> getSize() const { return size; }
		int getHealth() const { return health; }
		bool getIsActive() const { return isActive; }

		void setPos(std::pair<float, float> newPos) { pos = newPos; }
		void setSize(std::pair<float, float> newSize) { size = newSize; }
		void setHealth(int newHealth) { health = newHealth; }
		void setIsActive(bool active) { isActive = active; }
	protected:
		std::pair<float, float> pos;
		std::pair<float, float> size;
		bool isActive;
		int health;
		int maxHealth;
};

class SpeedUpBlock : public BaseBlock
{
	public:
		SpeedUpBlock(std::pair<float, float> pos, std::pair<float, float> size, int health, float speed_up)
			: BaseBlock(pos, size, health), speed_up(speed_up) {
		}

		void Draw(SDL_Renderer* renderer) const;
		bool getHit(Ball* ball);

		float getSpeedUp() const { return speed_up; }

		void setSpeedUp(float su) { speed_up = su; }
	private:
		float speed_up;
};

class InvincibleBlock : public BaseBlock
{
	public:
		InvincibleBlock(std::pair<float, float> pos, std::pair<float, float> size) 
			: BaseBlock(pos, size, -1) {}

		virtual void Draw(SDL_Renderer* renderer) const;
		virtual bool getHit(Ball* ball);
};

class BonusBlock : public BaseBlock
{
	public:
		BonusBlock(std::pair<float, float> pos, std::pair<float, float> size, int health)
			: BaseBlock(pos, size, health) {
		}

		void Draw(SDL_Renderer* renderer) const;
		virtual bool getHit(Ball* ball) = 0;
};

class MovementBonusBlock : public BonusBlock
{
	public:
		MovementBonusBlock(std::pair<float, float> pos, std::pair<float, float> size, int health, float delta_speed)
			: BonusBlock(pos, size, health), delta_speed(delta_speed) {}

		virtual
		float getDeltaSpeed() const { return delta_speed; }

		void setDeltaSpeed(float ds) { delta_speed = ds; }
	private:
		float delta_speed;

};