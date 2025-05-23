#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Ball.hpp"
#include "Slider.hpp"
#include "Block.hpp"
#include "Bonus.hpp"
#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>

class Field
{
	public:
		Field(Ball *ball, Slider *slider, int windowWidth, int windowHeight);
		Field(Ball* ball, Slider* slider, std::pair<int, int> windowSize);
		
		void Update(SideToSlide sliderMove, float deltaTime);
		void Draw(SDL_Renderer* renderer);
		
		bool isGameOver() const { return health <= 0; }
		int getScore() const { return score; }

		void reloadGame(int cols, int rows);
		void CreateRandomField(int cols, int rows);
		void addBlock(BaseBlock* bk);

		static void loadHealthIcon(SDL_Renderer* renderer, const std::string& path);
		static void setHealthSize(int hS) { health_size = hS; };

		static void loadFont(SDL_Renderer* renderer, const std::string& path);
		static void setFontSize(int fS) { font_size = fS; };

	private:
		void handleBallLost();
		void CleanDestroyedBlocks();
		void CleanDestroyedBonuses();
		Ball *ball;
		Slider *slider;
		
		std::vector<BaseBlock*> blocks;
		std::vector<Bonus*> droppedBonuses;
		std::pair<int, int> windowSize;

		int activeBonusCounter;
		float randomTrajectoryTimer = 0.0f;
		int score = 0;
		int health = 3;

		static TTF_Font* font;
		static int font_size;

		static SDL_Texture* health_icon;
		static int health_size;

		static constexpr float BLOCK_WIDTH = 60.0f;
		static constexpr float BLOCK_HEIGHT = 20.0f;
		static constexpr float BLOCK_PADDING = 5.0f;
		static constexpr float BLOCKS_TOP_MARGIN = 100.0f;
		
		static constexpr float RANDOM_TRAJECTORY_INTERVAL = 5.0f; 
		static constexpr float RANDOM_ANGLE_MAX = 30.0f;
};