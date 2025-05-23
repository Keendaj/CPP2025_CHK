#pragma once
#include <SDL2/SDL.h>
#include <utility>
#include <stdexcept>

enum class SideToSlide
{
	Slider_Left, Slider_Right, Stop
};

class Slider
{
	public:
		Slider(std::pair<float, float> pos,
			std::pair<float, float> size,
			float moveSpeed) 
			: pos(pos), 
			size(size), 
			moveSpeed(moveSpeed), 
			windowWidth(0), 
			windowHeight(0),
			sizeMultiplayer(1.0f) { }

		Slider(float xPos, 
			float yPos, 
			float xSize, 
			float ySize, 
			float moveSpeed)
			: pos({ xPos, yPos }), 
			size({ xSize, ySize }), 
			moveSpeed(moveSpeed),
			windowWidth(0), 
			windowHeight(0), 
			sizeMultiplayer(1.0f) { }

		void drawSlider(SDL_Renderer* renderer) const;
		void updateSlider(SideToSlide sideToMove, float deltaTime);

		std::pair<float, float> getPos() const { return pos; }
		void setPos(std::pair<float, float> newPos) { pos = newPos; }
		void setPos(float x, float y) { pos = { x, y }; }

		std::pair<float, float> getSize() const { return { size.first * sizeMultiplayer, size.second }; }
		void setSize(std::pair<float, float> newSize) { size = newSize; }
		void setSize(float xSize, float ySize) { size = { xSize, ySize }; }

		void setSizeMultiplayer(float sM) { sizeMultiplayer = sM;  }

		SDL_Rect getRect() const;

		void setWindowSize(int width, int height) { windowWidth = width; windowHeight = height; }
	private:
		std::pair<float, float> pos;
		std::pair<float, float> size;
		float sizeMultiplayer;
		float moveSpeed;
		int windowWidth;
		int windowHeight;
};