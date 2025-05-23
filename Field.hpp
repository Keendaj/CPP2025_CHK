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
		Field(Ball *ball, Slider* slider, std::pair<int, int> windowSize);
		~Field();

		void update(SideToSlide sliderMove, float deltaTime);
		void draw(SDL_Renderer* renderer);
		
		bool isGameOver() const { return health <= 0; }
		int getScore() const { return score; }

		void reloadGame(int cols, int rows);
		void createRandomField(int cols, int rows);
		void addBlock(BaseBlock* bk);

		bool getIsOneTimeNet() const { return isOneTimeNet; }
		void setIsOneTimeNet(bool otn) { isOneTimeNet = otn; }

		static void loadHealthIcon(SDL_Renderer* renderer, const std::string& path);
		static void destroyHealtIcon();
		static void setHealthSize(int hS) { healthSize = hS; };

		static void loadFont(SDL_Renderer* renderer, const std::string& path);
		static void destroyFont();
		static void setFontSize(int fS) { fontSize = fS; };

	private:
		void handleBallLost();
		void CleanDestroyedBlocks();
		void CleanDestroyedBonuses();
		Ball *ball;
		Slider *slider;
		
		std::vector<BaseBlock*> blocks;
		std::vector<Bonus*> droppedBonuses;
		std::pair<int, int> windowSize;

		bool isOneTimeNet;
		int activeBonusCounter;
		int score = 0;
		int health = 3;

		static TTF_Font* font;
		static int fontSize;

		static SDL_Texture* healthIcon;
		static int healthSize;

		static constexpr float BLOCK_WIDTH = 60.0f;
		static constexpr float BLOCK_HEIGHT = 20.0f;
		static constexpr float BLOCK_PADDING = 10.0f;
		static constexpr float BLOCKS_TOP_MARGIN = 100.0f;
};