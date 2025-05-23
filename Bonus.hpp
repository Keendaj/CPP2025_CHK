#pragma once
#include <utility>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Ball.hpp"
#include "Slider.hpp"


class Bonus
{
	public:
		Bonus(std::pair<float, float> pos)
			: pos(pos), isActive(false), isDropped(false) { }

		virtual void doBonus(Ball* ball, Slider* slider) = 0;
		virtual void removeBonus(Ball* ball, Slider* slider) = 0;

		void Update(float deltaTime);
		virtual void Draw(SDL_Renderer* renderer, int curBonusNumber) = 0;

		bool getIsActive() const { return isActive; }
		void setIsActive(bool isA) { isActive = isA; }

		bool getIsDropped() const { return isDropped; }
		void setIsDropped(bool isA) { isDropped = isA; }

		float getEndtime() const { return curEndtime;  }
		void resetEndtime() { curEndtime = maxEndtime; }

		SDL_Rect getRect() const;

		static void setStandartColor(SDL_Color color) { standartColor = color; }
		static void setFallSpeed(float speed) { fall_speed = speed; }
		static void setEndtime(float et) { maxEndtime = et; }
		static void setSize(float sz) {size = sz; }
		static void setPadding(int pd) { padding = pd; }

	protected:
		static SDL_Color standartColor;
		static float fall_speed;
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
		void doBonus(Ball* ball, Slider* slider);
		void removeBonus(Ball* ball, Slider* slider);

		void Draw(SDL_Renderer* renderer, int curBonusNumber);

		static void LoadTexture(SDL_Renderer* renderer, const std::string& path);
		static void DestroyTexture();
		static float setSizeMultiplier(float mult) { multiplier = mult; }

	protected:
		static float multiplier;
		static SDL_Texture* texture;
};

class BallSpeedBonus : public Bonus
{
	public:
		BallSpeedBonus(std::pair<float, float> pos) : Bonus(pos) { }

		void doBonus(Ball* ball, Slider* slider);
		void removeBonus(Ball* ball, Slider* slider);

		void Draw(SDL_Renderer* renderer, int curBonusNumber);

		static void LoadTexture(SDL_Renderer* renderer, const std::string& path);
		static void DestroyTexture();
		static float setSpeedMultiplier(float mult) { multiplier = mult; }
		
	protected:
		static float multiplier;
		static SDL_Texture* texture;
};

class StickyBonus : public Bonus
{
	public:
		StickyBonus(std::pair<float, float> pos) : Bonus(pos) { }

		void doBonus(Ball* ball, Slider* slider);
		void removeBonus(Ball* ball, Slider* slider);

		void Draw(SDL_Renderer* renderer, int curBonusNumber);

		static void LoadTexture(SDL_Renderer* renderer, const std::string& path);
		static void DestroyTexture();
	protected:
		static SDL_Texture* texture;
};