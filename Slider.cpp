#include "Slider.hpp"



void Slider::drawSlider(SDL_Renderer* renderer) const {
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

	float newPos = pos.first + movement;
	if (newPos < 0) {
		pos.first = 0;
	} 
	else if (newPos > windowWidth - size.first) {
		pos.first = windowWidth - size.first;
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