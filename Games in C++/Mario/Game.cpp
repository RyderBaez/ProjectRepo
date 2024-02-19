//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//
#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include <SDL2/SDL_image.h>
#include "Random.h"
#include <fstream>
#include "CollisionComponent.h"
#include "Block.h"
#include "Player.h"
#include "Goomba.h"
#include "Spawner.h"
#include "SDL2/SDL_mixer.h"

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
	{
		return false;
	}

	mWindow = SDL_CreateWindow("MAMA MIA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	LoadData();

	return (mRenderer != nullptr); //if renderer is true its good
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	UnloadData();
	IMG_Quit();
	Mix_CloseAudio();
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

	if (deltaTime > .033f) //checks for really long frames and corrects
	{
		deltaTime = static_cast<float>(.033f);
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

void Game::AddActor(Actor* a)
{
	mAllActors.push_back(a);
}

void Game::RemoveActor(Actor* a)
{
	std::vector<Actor*>::iterator it = find(mAllActors.begin(), mAllActors.end(), a);
	mAllActors.erase(it);
}

void Game::AddBlock(Block* b)
{
	mAllBlocks.push_back(b);
}

void Game::RemoveBlock(Block* b)
{
	std::vector<Block*>::iterator it = find(mAllBlocks.begin(), mAllBlocks.end(), b);
	mAllBlocks.erase(it);
}

void Game::AddGoomba(Goomba* g)
{
	mAllGoombas.push_back(g);
}

void Game::RemoveGoomba(Goomba* g)
{
	std::vector<Goomba*>::iterator it = find(mAllGoombas.begin(), mAllGoombas.end(), g);
	mAllGoombas.erase(it);
}

void Game::LoadData()
{
	Actor* background = new Actor(this);
	SpriteComponent* sc = new SpriteComponent(background, 80);
	sc->SetTexture(GetTexture("Assets/Background.png"));
	background->SetPosition(Vector2(BACKGROUND_X, BACKGROUND_Y));

	std::ifstream infile("Assets/Level1.txt");
	std::string line;

	float xPos = FILE_OFFSET;
	float yPos = FILE_OFFSET;
	while (std::getline(infile, line)) //while more file to read
	{
		for (size_t i = 0; i < line.length(); i++)
		{

			if (line[i] <= 'I' && line[i] >= 'A')
			{
				Actor* block = new Block(this, line[i]);

				block->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
			}
			switch (line[i])
			{

			case ('P'):
			{
				mPlayer = new Player(this);

				mPlayer->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
				break;
			}

			case ('Y'):
			{
				Actor* spawner = new Spawner(this);
				spawner->SetPosition(Vector2(xPos + (static_cast<float>(TILE_SIZE) * i), yPos));
				break;
			}
			}
		}
		yPos += static_cast<float>(TILE_SIZE);
	}
	mBackgroundMusic = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
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
	std::unordered_map<std::string, Mix_Chunk*>::iterator itMix = mMixChunkMap.begin();
	while (itMix != mMixChunkMap.end())
	{
		Mix_FreeChunk(itMix->second);
		itMix++;
	}
	mMixChunkMap.clear();
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

const std::vector<Block*>& Game::GetAllBlocks()
{
	return mAllBlocks;
}

const std::vector<Goomba*>& Game::GetAllGoombas()
{
	return mAllGoombas;
}

const Actor* Game::GetPlayer()
{
	return mPlayer;
}

Vector2& Game::GetCameraPos()
{
	return mCameraPosition;
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	if (mMixChunkMap.find(fileName) != mMixChunkMap.end())
	{
		return mMixChunkMap[fileName];
	}

	const char* file = fileName.c_str();
	Mix_Chunk* newSound = Mix_LoadWAV(file);

	if (newSound == nullptr)
	{
		SDL_Log("Audio Failed to Load");
	}
	else
	{
		mMixChunkMap.insert({fileName, newSound});
	}

	return newSound;
}

const int Game::GetBackgroundMusicChannel() const
{
	return mBackgroundMusic;
}