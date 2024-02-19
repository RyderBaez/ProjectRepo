#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include "Math.h"

const int WINDOW_WIDTH = 448;
const int WINDOW_LENGTH = 512;
const float FROGGER_X_ALIGNMENT = 32.0f;
const float FROGGER_Y_ALIGNMENT = 80.0f;
const int TILE_SIZE = 32;
const int WATER_LEVEL = 255;
class Game
{
public:
	bool Initialize();
	void Shutdown();
	void RunLoop();

	void AddActor(class Actor* a);
	void RemoveActor(Actor* a);

	void AddSprite(class SpriteComponent* s);
	void RemoveSprite(SpriteComponent* s);

	void AddVehicle(class Vehicle* v);
	void RemoveVehicle(Vehicle* v);

	void AddLog(class Log* l);
	void RemoveLog(Log* l);

	Actor* GetGoal();

	Vector2 GetFrogPosition();

	const std::vector<Vehicle*>& GetAllVehicles();

	const std::vector<Log*>& GetAllLogs();

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
	std::vector<Vehicle*> mAllVehicles;
	std::vector<Log*> mAllLogs;
	std::unordered_map<std::string, SDL_Texture*> mSpriteMap;

	Uint32 mPreviousTime = 0;

	Actor* mFrog = nullptr;
	Actor* mGoal = nullptr;
};
