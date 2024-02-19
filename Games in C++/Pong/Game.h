#pragma once
#include "SDL2/SDL.h"
const int WINDOW_WIDTH = 1024;
const int WINDOW_LENGTH = 768;

const int WALL_THICKNESS = 16;
const int PADDLE_LENGTH = 128;
const int PADDLE_OFFSET = 10;
const int BALL_SIZE = 16;
const int MOVE = 1;
const int PADDLE_SPEED = 1000;

class Game
{
public:
	bool Initialize();
	void Shutdown();
	void RunLoop();

private:
	bool mGameContinue = true;
	bool mNoGameOver = true;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	bool CreateRect(int x, int y, int width, int length);

	void MovePaddle(float deltaTime);
	void MoveBall(float deltaTime);
	void BallPaddleConflict();

	void ProcessInput();
	void GenerateOutput();
	void UpdateGame();

	SDL_Point mPaddlePoint = {PADDLE_OFFSET, (WINDOW_LENGTH - PADDLE_LENGTH) / 2};

	SDL_Point mBallPoint = {(WINDOW_WIDTH / 2) - (BALL_SIZE / 2),
							(WINDOW_LENGTH / 2) - (BALL_SIZE / 2)};

	SDL_Point mBallVelocity{(PADDLE_SPEED / 3), (PADDLE_SPEED / 2)};

	int mPaddleDirection = 0;
	Uint32 mPreviousTime = 0;
};
