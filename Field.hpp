#pragma once
#include <SDL2/SDL.h>
#include "Ball.hpp"
#include "Slider.hpp"
#include <vector>
#include <utility>
#include <random>

class Field
{
	public:
		Field(Ball *ball, Slider *slider, int windowWidth, int windowHeight);
		Field(Ball* ball, Slider* slider, std::pair<int, int> windowSize);
		~Field();
		
		void Update(float deltaTime);
		void Draw(SDL_Renderer* renderer);
		void InitializeBlocks(int rows, int cols);
		bool isGameOver() const { return health <= 0; }
		int getScore() const { return score; }

	private:
		bool checkBallCollisionWithRect(SDL_Rect *rect);
		void cleanupDestroyedBlocks();
		void updateRandomTrajectory(float deltaTime);
		void handleBallLost();

		Ball *ball;
		Slider *slider;
		
		std::pair<int, int> windowSize;
		bool isBallSticky = false;
		bool hasSafetyNet = false;
		float randomTrajectoryTimer = 0.0f;
		int score = 0;
		int health = 3;
		
		static constexpr float BLOCK_WIDTH = 60.0f;
		static constexpr float BLOCK_HEIGHT = 20.0f;
		static constexpr float BLOCK_PADDING = 5.0f;
		static constexpr float BLOCKS_TOP_MARGIN = 50.0f;
		
		static constexpr float RANDOM_TRAJECTORY_INTERVAL = 5.0f; 
		static constexpr float RANDOM_ANGLE_MAX = 30.0f;
};