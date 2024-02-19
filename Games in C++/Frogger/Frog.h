#pragma once
#include "Actor.h"
#include <map>
const float MOVE_AMOUNT = 32.0f;
const float FROGGER_MIN = 80.0f;
const float FROGGER_MAX_OFFSET = 48.0f;
const float FROG_DEFAULT_X = 32.0f * 7.0f;
const float FROG_DEFAULT_Y = 32.0f * 12.0f + 80.0f;
const int FROG_SIZE = 25;
class Frog : public Actor
{
public:
	Frog(class Game* game);
	~Frog();

private:
	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState) override;
	//class MoveComponent* mMove = nullptr;
	class SpriteComponent* mSprite = nullptr;
	class CollisionComponent* mCollider = nullptr;
	std::map<int, bool> mPrevInputs;
	void GetButtonState(const Uint8* keyState, int button);
};