#pragma once
#include <utility>
#include <SDL2/SDL.h>

class Ball
{
	public:
		Ball(float x, float y, float radius) 
			: pos({x, y}), velocity({0, -200}), radius(radius), baseSpeed(200.0f) {};
		Ball(std::pair<float, float> pos, float radius) 
			: pos(pos), velocity({0, -200}), radius(radius), baseSpeed(200.0f) {};

		void drawBall(SDL_Renderer *renderer);
		void updateBall(float delta_time);
		void launch(float angle);
		bool checkCollisionWithRect(SDL_Rect* rect);

		std::pair<float, float> getPos() const { return pos; };
		void setPos(float x, float y) { this->pos = { x, y }; };
		void setPos(std::pair<float, float> pos) { this->pos = pos; };
		
		std::pair<float, float> getVelocity() const { return velocity; };
		void setVelocity(float x_velocity, float y_velocity);
		void setVelocity(std::pair<float, float> velocity);

		float getSize() const { return radius; };
		void setSize(float radius) { this->radius = radius; };

		void setSpeed(float speed) { baseSpeed = speed; }
		float getSpeed() const { return baseSpeed; }

		bool getSticky() const { return isSticky; }
		void setSticky(bool isSticky) { this->isSticky = isSticky; }

		void setWindowSize(int width, int height) { windowWidth = width; windowHeight = height; }

	private:
		std::pair<float, float> pos;
		std::pair<float, float> velocity;
		bool isSticky;
		float radius;
		float baseSpeed;
		int windowWidth;
		int windowHeight;

		void normalizeVelocity();
};

