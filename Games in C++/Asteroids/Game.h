#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>

const int WINDOW_WIDTH = 1024;
const int WINDOW_LENGTH = 768;

class Game
{
public:
	bool Initialize();
	void Shutdown();
	void RunLoop();

	void AddActor(class Actor* a);
	void RemoveActor(Actor* a);
	void AddAsteroid(class Asteroid* a);
	void RemoveAsteroid(Asteroid* a);
	void AddSprite(class SpriteComponent* s);
	void RemoveSprite(SpriteComponent* s);
	const std::vector<Asteroid*> GetAsteroids();
	SDL_Texture* GetTexture(std::string fileName);

private:
	void ProcessInput();
	void GenerateOutput();
	void UpdateGame();
	void LoadData();
	void UnloadData();

	bool mGameContinue = true;
	bool mNoGameOver = true;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	std::vector<Actor*> mAllActors;
	std::vector<SpriteComponent*> mSpriteComps;
	std::unordered_map<std::string, SDL_Texture*> mSpriteMap;
	std::vector<Asteroid*> mAllAsteroids;

	Uint32 mPreviousTime = 0;
};
