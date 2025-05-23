#pragma once
#include <SDL2/SDL.h>
#include "Bonus.hpp"
#include "Ball.hpp"
#include <utility>
#include <vector>



class BaseBlock
{
	public:
		BaseBlock(std::pair<float, float> pos,
			std::pair<float, float> size, 
			int health)
			: pos(pos),
			size(size),
			health(health),
			maxHealth(health),
			isActive(true) { }

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
		SpeedUpBlock(std::pair<float, float> pos,
			std::pair<float, float> size,
			int health, 
			float speed_up)
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

		void Draw(SDL_Renderer* renderer) const;
		bool getHit(Ball* ball);
};

class BonusBlock : public BaseBlock
{
	public:
		BonusBlock(std::pair<float, float> pos,
			std::pair<float, float> size, 
			int health, 
			Bonus* bonus)
			: BaseBlock(pos, size, health), bonus(bonus) { }

		void Draw(SDL_Renderer* renderer) const;
		bool getHit(Ball* ball);

		Bonus* getBonus() const { return bonus; }
		void setBonus(Bonus* bs) { bonus = bs; }

	protected:
		Bonus* bonus;
};

//class MovingBlock : public BaseBlock {
//	public:
//		MovingBlock(std::pair<float, float> pos,
//			std::pair<float, float> size,
//			int health, 
//			int speed,
//			int leftBorder,
//			int rightBorder)
//			: BaseBlock(pos, size, health),
//			speed(speed),
//			direction(1),
//			leftBorder(leftBorder),
//			rightBorder(rightBorder) { }
//
//		int getDirection() const { return direction; }
//		void changeDirection() { direction *= -1; }
//
//	protected:
//		int speed;
//		int direction;
//		int leftBorder;
//		int rightBorder;
//};