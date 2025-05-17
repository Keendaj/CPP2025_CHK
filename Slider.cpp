#include "Slider.hpp"
#include <iostream>

void Slider::drawSlider(SDL_Renderer* renderer) const {
	if (!renderer) return;

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	SDL_Rect slider = getRect();
	SDL_RenderFillRect(renderer, &slider);
}

void Slider::updateSlider(SideToSlide side_to_move, float delta_time) {
	float movement = 0.0f;

	switch (side_to_move) {
		case SideToSlide::Slider_Left:
			movement = -moveSpeed * delta_time;
			break;
		case SideToSlide::Slider_Right:
			movement = moveSpeed * delta_time;
			break;
		case SideToSlide::Stop:
			return;
	}

	float maxX = static_cast<float>(windowWidth) - size.first;
	float newPos = pos.first + movement;
	if (newPos < 0.0f) {
		pos.first = 0.0f;
	} 
	else if (newPos > maxX) {
		pos.first = maxX;
	} 
	else {
		pos.first = newPos;
	}
}

SDL_Rect Slider::getRect() const {
	return SDL_Rect{
		static_cast<int>(pos.first),
		static_cast<int>(pos.second),
		static_cast<int>(size.first),
		static_cast<int>(size.second)
	};
}