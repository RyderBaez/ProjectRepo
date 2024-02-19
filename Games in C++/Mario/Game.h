#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include "Math.h"

class Actor;
class SpriteComponent;
class Block;
class Goomba;
class Player;
struct Mix_Chunk;

const int WINDOW_WIDTH = 600;
const int WINDOW_LENGTH = 448;
const int TILE_SIZE = 32;
const float FILE_OFFSET = 16.0f;
const int BACKGROUND_X = 3392;
const int BACKGROUND_Y = 224;
const int SCREEN_OFFSET = 300;
const int FLAG_POSITION = 6368;

class Game
{
public:
	bool Initialize();
	void Shutdown();
	void RunLoop();

	void AddActor(Actor* a);
	void RemoveActor(Actor* a);

	void AddSprite(SpriteComponent* s);
	void RemoveSprite(SpriteComponent* s);

	void AddBlock(Block* b);
	void RemoveBlock(Block* b);

	void AddGoomba(Goomba* g);
	void RemoveGoomba(Goomba* g);

	const std::vector<Block*>& GetAllBlocks();
	const std::vector<Goomba*>& GetAllGoombas();
	const Actor* GetPlayer();

	Actor* GetGoal();

	Vector2& GetCameraPos();

	SDL_Texture* GetTexture(std::string fileName);

	Mix_Chunk* GetSound(const std::string& fileName);

	const int GetBackgroundMusicChannel() const;

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

	Vector2 mCameraPosition;

	std::vector<Actor*> mAllActors;
	std::vector<SpriteComponent*> mSpriteComps;
	std::vector<Goomba*> mAllGoombas;
	std::vector<Block*> mAllBlocks;
	std::unordered_map<std::string, SDL_Texture*> mSpriteMap;
	std::unordered_map<std::string, Mix_Chunk*> mMixChunkMap;
	Actor* mPlayer = nullptr;

	Uint32 mPreviousTime = 0;

	int mBackgroundMusic = 0;
};
