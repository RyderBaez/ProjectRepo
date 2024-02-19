//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
	{
		return false;
	}

	mWindow = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
							   WINDOW_LENGTH, 0);

	if (mWindow == nullptr)
	{
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	return (mRenderer != nullptr); //if renderer is true its good
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mGameContinue && mNoGameOver)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event currentEvent;
	while (SDL_PollEvent(&currentEvent))
	{
		switch (currentEvent.type)
		{

		case SDL_QUIT:
			mGameContinue = false;
			return;
			break;

		default:
			break;
		}
	}
	const Uint8* pressedKeys = SDL_GetKeyboardState(nullptr);

	if (pressedKeys[SDL_SCANCODE_ESCAPE] != 0)
	{
		mGameContinue = false;
		return;
	}

	if (pressedKeys[SDL_SCANCODE_W] != 0)
	{
		mPaddleDirection += MOVE; //add up component
	}

	if (pressedKeys[SDL_SCANCODE_S] != 0)
	{
		mPaddleDirection -= MOVE; //adds down component
	}
}

void Game::UpdateGame()
{
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = 0;

	while (currentTime - mPreviousTime < 16) //checks for too short of frames and corrects
	{
		currentTime = SDL_GetTicks();
	}
	deltaTime = static_cast<float>((currentTime - mPreviousTime) / 1000.0);

	if (deltaTime > .033) //checks for really long frames and corrects
	{
		deltaTime = static_cast<float>(.033);
	}
	mPreviousTime = currentTime;

	MovePaddle(deltaTime);
	MoveBall(deltaTime);
	BallPaddleConflict();

	if (mBallPoint.x + BALL_SIZE <= 0)
	{
		mNoGameOver = false;
		return; //ends game if ball goes off the screen
	}
}

void Game::GenerateOutput()
{
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	if (SDL_RenderFillRect(mRenderer, nullptr) != 0)
	{
		mGameContinue = false;
		return;
	} //fills the background color

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	if (!CreateRect(0, 0, WINDOW_WIDTH, WALL_THICKNESS)) //creates the top wall
	{
		mGameContinue = false;
		return;
	}

	if (!CreateRect(WINDOW_WIDTH - WALL_THICKNESS, 0, WALL_THICKNESS,
					WINDOW_LENGTH)) //creates the right wall
	{
		mGameContinue = false;
		return;
	}

	if (!CreateRect(0, WINDOW_LENGTH - WALL_THICKNESS, WINDOW_WIDTH,
					WALL_THICKNESS)) //creates the bottom wall
	{
		mGameContinue = false;
		return;
	}

	//draws the state of the paddle and the ball
	if (!CreateRect(mPaddlePoint.x, mPaddlePoint.y, BALL_SIZE, PADDLE_LENGTH)) //creates the Paddle
	{
		mGameContinue = false;
		return;
	}

	if (!CreateRect(mBallPoint.x, mBallPoint.y, BALL_SIZE,
					BALL_SIZE)) //creates the Ball
	{
		mGameContinue = false;
		return;
	}

	SDL_RenderPresent(mRenderer);
}

bool Game::CreateRect(int x, int y, int width,
					  int length) //creates the rectangle in 1 function
{
	SDL_Rect temp;
	temp.x = x;
	temp.y = y;
	temp.w = width;
	temp.h = length;

	return (SDL_RenderFillRect(mRenderer, &temp) ==
			0); //returns if the rectangle was properly drawn
}

void Game::MovePaddle(float deltaTime) //controls moving the paddle for game update
{
	if (mPaddleDirection < 0)
	{
		mPaddlePoint.y += static_cast<int>(PADDLE_SPEED * deltaTime); //larger number is down
	}

	if (mPaddleDirection > 0)
	{
		mPaddlePoint.y -= static_cast<int>(PADDLE_SPEED * deltaTime); //smaller is up!
	}

	if (mPaddlePoint.y < WALL_THICKNESS)
	{
		mPaddlePoint.y = WALL_THICKNESS;
	}

	if (mPaddlePoint.y + PADDLE_LENGTH > WINDOW_LENGTH - WALL_THICKNESS)
	{
		mPaddlePoint.y = WINDOW_LENGTH - WALL_THICKNESS - PADDLE_LENGTH;
	}

	mPaddleDirection = 0;
}

void Game::MoveBall(float deltaTime) //controls moving the ball for game update
{
	mBallPoint.x += static_cast<int>(mBallVelocity.x * deltaTime);
	mBallPoint.y += static_cast<int>(mBallVelocity.y * deltaTime);

	if (mBallPoint.y < WALL_THICKNESS) //hitting the top wall
	{
		mBallPoint.y = WALL_THICKNESS;
		mBallVelocity.y = mBallVelocity.y * -1;
	}

	if (mBallPoint.y + BALL_SIZE > WINDOW_LENGTH - WALL_THICKNESS) //hitting the bottom wall
	{
		mBallPoint.y = WINDOW_LENGTH - WALL_THICKNESS - BALL_SIZE;
		mBallVelocity.y = mBallVelocity.y * -1;
	}

	if (mBallPoint.x + BALL_SIZE > WINDOW_WIDTH - WALL_THICKNESS) //hitting the right wall
	{
		mBallPoint.x = WINDOW_WIDTH - WALL_THICKNESS - BALL_SIZE;
		mBallVelocity.x = mBallVelocity.x * -1;
	}
}

void Game::BallPaddleConflict() //controls the ball hitting the paddle
{
	if (mPaddlePoint.x <= mBallPoint.x &&
		mBallPoint.x <= (mPaddlePoint.x + BALL_SIZE)) //checks if top of ball is within paddle area
	{
		if (mPaddlePoint.y <= mBallPoint.y && mBallPoint.y <= (mPaddlePoint.y + PADDLE_LENGTH))
		{
			mBallPoint.x = mPaddlePoint.x + BALL_SIZE;
			mBallVelocity.x = mBallVelocity.x * -1;
		}
		else if (mPaddlePoint.y <= mBallPoint.y + BALL_SIZE &&
				 mBallPoint.y + BALL_SIZE <= (mPaddlePoint.y + PADDLE_LENGTH))
		{
			mBallPoint.x = mPaddlePoint.x + BALL_SIZE;
			mBallVelocity.x = mBallVelocity.x * -1;
		}
	}

	if (mPaddlePoint.x <= mBallPoint.x + BALL_SIZE &&
		mBallPoint.x + BALL_SIZE <=
			(mPaddlePoint.x + BALL_SIZE)) //checks for the bottom of the ball
	{
		if (mPaddlePoint.y <= mBallPoint.y && mBallPoint.y <= (mPaddlePoint.y + PADDLE_LENGTH))
		{
			mBallPoint.x = mPaddlePoint.x + BALL_SIZE;
			mBallVelocity.x = mBallVelocity.x * -1;
		}
		else if (mPaddlePoint.y <= mBallPoint.y + BALL_SIZE &&
				 mBallPoint.y + BALL_SIZE <= (mPaddlePoint.y + PADDLE_LENGTH))
		{
			mBallPoint.x = mPaddlePoint.x + BALL_SIZE;
			mBallVelocity.x = mBallVelocity.x * -1;
		}
	}
}