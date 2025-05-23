#pragma once
#include <utility>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include "Ball.hpp"
#include "Slider.hpp"

class Field;

class Bonus
{
	public:
		Bonus(std::pair<float, float> pos)
			: pos(pos), 
			isActive(false), 
			isDropped(false) { }

		virtual ~Bonus() = default;
		virtual void doBonus(Ball* ball, Slider* slider, Field* field) = 0;
		virtual void removeBonus(Ball* ball, Slider* slider, Field* field) = 0;

		void update(float deltaTime);
		virtual void draw(SDL_Renderer* renderer, int curBonusNumber) = 0;

		bool getIsActive() const { return isActive; }
		void setIsActive(bool isA) { isActive = isA; }

		bool getIsDropped() const { return isDropped; }
		void setIsDropped(bool isA) { isDropped = isA; }

		float getEndtime() const { return curEndtime;  }
		void resetEndtime() { curEndtime = maxEndtime; }

		SDL_Rect getRect() const;

		static void setStandartColor(SDL_Color color) { standartColor = color; }
		static void setFallSpeed(float speed) { fallSpeed = speed; }
		static void setEndtime(float et) { maxEndtime = et; }
		static void setSize(float sz) {size = sz; }
		static void setPadding(int pd) { padding = pd; }

	protected:
		static SDL_Color standartColor;
		static float fallSpeed;
		static float maxEndtime;
		static float size;
		static int padding;

		bool isActive;
		bool isDropped;

		float curEndtime;
		
		std::pair<float, float> pos;
		
};

class SliderSizeBonus : public Bonus
{
	public:
		SliderSizeBonus(std::pair<float, float> pos) : Bonus(pos) { }
		void doBonus(Ball* ball, Slider* slider, Field* field);
		void removeBonus(Ball* ball, Slider* slider, Field* field);

		void draw(SDL_Renderer* renderer, int curBonusNumber);

		static void loadTexture(SDL_Renderer* renderer, const std::string& path);
		static void destroyTexture();
		static float setSizeMultiplier(float mult) { multiplier = mult; }

	protected:
		static float multiplier;
		static SDL_Texture* texture;
};

class BallSpeedBonus : public Bonus
{
	public:
		BallSpeedBonus(std::pair<float, float> pos) : Bonus(pos) { }

		void doBonus(Ball* ball, Slider* slider, Field* field);
		void removeBonus(Ball* ball, Slider* slider, Field* field);

		void draw(SDL_Renderer* renderer, int curBonusNumber);

		static void loadTexture(SDL_Renderer* renderer, const std::string& path);
		static void destroyTexture();
		static float setSpeedMultiplier(float mult) { multiplier = mult; }
		
	protected:
		static float multiplier;
		static SDL_Texture* texture;
};

class StickyBonus : public Bonus
{
	public:
		StickyBonus(std::pair<float, float> pos) : Bonus(pos) { }

		void doBonus(Ball* ball, Slider* slider, Field* field) {};
		void removeBonus(Ball* ball, Slider* slider, Field* field) {};

		void draw(SDL_Renderer* renderer, int curBonusNumber);

		static void loadTexture(SDL_Renderer* renderer, const std::string& path);
		static void destroyTexture();
	protected:
		static SDL_Texture* texture;
};

class MovingBlockBonus : public Bonus
{
	public:
		MovingBlockBonus(std::pair<float, float> pos,
			int blockSpeed,
			int blockHealth,
			int leftBorder,
			int rightBorder,
			std::pair<int, int> blockPos,
			std::pair<int, int> blockSize)
			: Bonus(pos),
			blockSpeed(blockSpeed),
			blockHealth(blockHealth),
			leftBorder(leftBorder),
			rightBorder(rightBorder),
			blockPos(blockPos),
			blockSize(blockSize){}

		void doBonus(Ball* ball, Slider* slider, Field* field);
		void removeBonus(Ball* ball, Slider* slider, Field* field) {};

		void draw(SDL_Renderer* renderer, int curBonusNumber);

		static void loadTexture(SDL_Renderer* renderer, const std::string& path);
		static void destroyTexture();
	protected:
		int blockSpeed;
		int leftBorder;
		int rightBorder;
		int blockHealth;
		std::pair<int, int> blockPos;
		std::pair<int, int> blockSize;

		static SDL_Texture* texture;
};