#pragma once
#include "Actor.h"
#include <string>
const float LOG_SPEED = 37.5f;
const int LOG_SHORT = 96;
const int LOG_MEDIUM = 128;
const int LOG_LONG = 192;
class Log : public Actor
{
public:
	Log(class Game* game, char type, float direction);
	~Log();
	class CollisionComponent* GetCollisionComponent();
	class WrappingMove* GetMovementComponent();

private:
	//void OnUpdate(float deltaTime) override;
	class WrappingMove* mMove = nullptr;
	class SpriteComponent* mSprite = nullptr;
	class CollisionComponent* mCollider = nullptr;
};