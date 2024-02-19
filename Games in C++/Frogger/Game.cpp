//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//
#include "Vehicle.h"
#include "Frog.h"
#include "Log.h"
#include "Game.h"
#include "Actor.h"
#include "WrappingMove.h"
#include "SpriteComponent.h"
#include <SDL2/SDL_image.h>
#include "Random.h"
#include <fstream>
#include "CollisionComponent.h"
bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
	{
		return false;
	}

	mWindow = SDL_CreateWindow("NO FROGGY NOOOO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   WINDOW_WIDTH, WINDOW_LENGTH, 0);
	if (mWindow == nullptr)
	{
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		return false;
	}
	Random::Init();
	LoadData();

	return (mRenderer != nullptr); //if renderer is true its good
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	UnloadData();
	IMG_Quit();
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

	std::vector<Actor*> tempVector = mAllActors;
	for (size_t i = 0; i < tempVector.size(); i++)
	{
		tempVector[i]->ProcessInput(pressedKeys);
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

	std::vector<Actor*> tempVector = mAllActors;
	std::vector<Actor*> tempForDestroy;

	for (size_t i = 0; i < tempVector.size(); i++)
	{
		tempVector[i]->Update(deltaTime);
		if (tempVector[i]->GetState() == ActorState::Destroy)
		{
			tempForDestroy.push_back(tempVector[i]);
		}
	}

	for (size_t i = 0; i < tempForDestroy.size(); i++)
	{
		delete tempForDestroy[i];
	}
}

void Game::GenerateOutput()
{
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	if (SDL_RenderFillRect(mRenderer, NULL) != 0)
	{
		mGameContinue = false;
		return;
	} //fills the background color

	for (size_t i = 0; i < mSpriteComps.size(); i++)
	{
		if (mSpriteComps[i]->IsVisible())
		{
			mSpriteComps[i]->Draw(mRenderer);
		}
	}

	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(class Actor* a)
{
	mAllActors.push_back(a);
}

void Game::RemoveActor(class Actor* a)
{
	std::vector<Actor*>::iterator it = find(mAllActors.begin(), mAllActors.end(), a);
	mAllActors.erase(it);
}

void Game::AddVehicle(class Vehicle* v)
{
	mAllVehicles.push_back(v);
}

void Game::RemoveVehicle(class Vehicle* v)
{
	std::vector<Vehicle*>::iterator it = find(mAllVehicles.begin(), mAllVehicles.end(), v);
	mAllVehicles.erase(it);
}

void Game::AddLog(class Log* l)
{
	mAllLogs.push_back(l);
}

void Game::RemoveLog(Log* l)
{
	std::vector<Log*>::iterator it = find(mAllLogs.begin(), mAllLogs.end(), l);
	mAllLogs.erase(it);
}

void Game::LoadData()
{
	Actor* background = new Actor(this);
	SpriteComponent* sc = new SpriteComponent(background, 80);
	sc->SetTexture(GetTexture("Assets/Background.png"));
	background->SetPosition(Vector2(WINDOW_WIDTH / 2.0f, WINDOW_LENGTH / 2.0f));

	std::ifstream infile("Assets/Level.txt");
	std::string line;

	float xPos = FROGGER_X_ALIGNMENT;
	float yPos = FROGGER_Y_ALIGNMENT;
	float direction = 1.0f;
	while (std::getline(infile, line)) //while more file to read
	{
		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == 'A')
			{
				Actor* vehicleA = new Vehicle(this, line[i], direction);

				vehicleA->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}

			if (line[i] == 'B')
			{
				Actor* vehicleB = new Vehicle(this, line[i], direction);

				vehicleB->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}

			if (line[i] == 'C')
			{
				Actor* vehicleC = new Vehicle(this, line[i], direction);

				vehicleC->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}

			if (line[i] == 'D')
			{
				Actor* vehicleD = new Vehicle(this, line[i], direction);

				vehicleD->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}

			if (line[i] == 'T')
			{
				Actor* truck = new Vehicle(this, line[i], direction);

				truck->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}

			if (line[i] == 'F')
			{
				mFrog = new Frog(this);

				mFrog->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}
			if (line[i] == 'G')
			{
				mGoal = new Actor(this);

				mGoal->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));

				CollisionComponent* colliderG = new CollisionComponent(mGoal);
				colliderG->SetSize(TILE_SIZE, TILE_SIZE);
			}

			if (line[i] == 'X')
			{
				Actor* logX = new Log(this, line[i], direction);

				logX->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}

			if (line[i] == 'Y')
			{
				Actor* logY = new Log(this, line[i], direction);

				logY->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}

			if (line[i] == 'Z')
			{
				Actor* logZ = new Log(this, line[i], direction);

				logZ->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}
		}
		direction = direction * -1;
		yPos += static_cast<float>(TILE_SIZE);
	}
	infile.close();
}

void Game::UnloadData()
{
	for (size_t i = 0; i < mAllActors.size(); i++)
	{
		delete mAllActors.back();
	}

	std::unordered_map<std::string, SDL_Texture*>::iterator it = mSpriteMap.begin();
	while (it != mSpriteMap.end())
	{
		SDL_DestroyTexture(it->second);
		it++;
	}

	mSpriteMap.clear();
}

SDL_Texture* Game::GetTexture(std::string fileName)
{
	if (mSpriteMap.find(fileName) != mSpriteMap.end())
	{
		return mSpriteMap[fileName];
	}

	const char* file = fileName.c_str();
	SDL_Surface* loadIn = IMG_Load(file);

	SDL_Texture* newTexture = nullptr;

	if (loadIn == nullptr)
	{
		SDL_Log("Sprite Failed to Load");
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(mRenderer, loadIn);
		SDL_FreeSurface(loadIn);
		mSpriteMap.insert({fileName, newTexture});
	}

	return newTexture;
}

void Game::AddSprite(class SpriteComponent* s)
{
	mSpriteComps.push_back(s);
}

void Game::RemoveSprite(class SpriteComponent* s)
{
	std::vector<SpriteComponent*>::iterator it = find(mSpriteComps.begin(), mSpriteComps.end(), s);
	mSpriteComps.erase(it);
}

Vector2 Game::GetFrogPosition()
{
	return mFrog->GetPosition();
}

Actor* Game::GetGoal()
{
	return mGoal;
}

const std::vector<Vehicle*>& Game::GetAllVehicles()
{
	return mAllVehicles;
}

const std::vector<Log*>& Game::GetAllLogs()
{
	return mAllLogs;
}