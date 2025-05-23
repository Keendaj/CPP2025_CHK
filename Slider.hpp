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
		Slider(std::pair<float, float> pos, std::pair<float, float> size, float moveSpeed) 
			: pos(pos), size(size), moveSpeed(moveSpeed), windowWidth(0), 
			windowHeight(0), isOneTimeNet(false), sizeMultiplayer(1.0f) { }

		Slider(float x_pos, float y_pos, float x_size, float y_size, float moveSpeed)
			: pos({ x_pos, y_pos }), size({ x_size, y_size }), moveSpeed(moveSpeed),
			 windowWidth(0), windowHeight(0), isOneTimeNet(false), sizeMultiplayer(1.0f) { }

		void drawSlider(SDL_Renderer* renderer) const;
		void updateSlider(SideToSlide side_to_move, float delta_time);

		std::pair<float, float> getPos() const { return pos; }
		void setPos(std::pair<float, float> new_pos) { pos = new_pos; }
		void setPos(float x, float y) { pos = { x, y }; }

		std::pair<float, float> getSize() const { return { size.first * sizeMultiplayer, size.second }; }
		void setSize(std::pair<float, float> new_size) { size = new_size; }
		void setSize(float x_size, float y_size) { size = { x_size, y_size }; }

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
		bool isOneTimeNet;
};