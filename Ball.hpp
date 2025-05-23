#pragma once
#include <utility>
#include <SDL2/SDL.h>

class Ball
{
	public:
		Ball(float x,
			float y,
			float xVelocity,
			float yVelocity, 
			float baseSpeed, 
			float radius,
			bool isSticky,
			SDL_Color ballColor);
		Ball(std::pair<float, float> pos,
			std::pair<float, float> velocity,
			float baseSpeed,
			float radius,
			bool isSticky,
			SDL_Color color);

		void drawBall(SDL_Renderer *renderer);
		void updateBall(float deltaTime);
		void launch(float angle);
		bool checkCollisionWithRect(SDL_Rect* rect);

		std::pair<float, float> getPos() const { return pos; };
		void setPos(float x, float y) { this->pos = { x, y }; };
		void setPos(std::pair<float, float> pos) { this->pos = pos; };
		
		std::pair<float, float> getVelocity() const { return velocity; };
		void setVelocity(float xVelocity, float yVelocity);
		void setVelocity(std::pair<float, float> newVelocity);

		float getSize() const { return radius; };
		void setSize(float radius) { this->radius = radius; };

		void setSpeed(float speed) { baseSpeed = speed; normalizeVelocity(); }
		float getSpeed() const { return baseSpeed*speedMultiplaer; }

		bool getSticky() const { return isSticky; }
		void setSticky(bool sticky) { this->isSticky = sticky; }

		float getSpeedMutiplayer() const { return speedMultiplaer; }
		void setSpeedMultiplier(float mult) { speedMultiplaer = mult; }

		void setWindowSize(int width, int height) { windowWidth = width; windowHeight = height; }

	private:
		std::pair<float, float> pos;
		std::pair<float, float> velocity;
		SDL_Color color;

		bool isSticky;
		float radius;
		float baseSpeed;
		float speedMultiplaer;
		int windowWidth;
		int windowHeight;

		void normalizeVelocity();
};

