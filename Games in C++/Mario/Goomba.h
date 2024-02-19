#pragma once
#include "Actor.h"

class AnimatedSprite;
class CollisionComponent;
class GoombaMove;

const float DEATH_SPRITE = 0.25f;
class Goomba : public Actor
{
public:
	Goomba(class Game* game);
	~Goomba();
	void Stomp();

private:
	void OnUpdate(float deltaTime) override;
	//class MoveComponent* mMove = nullptr;
	AnimatedSprite* mSprite = nullptr;
	CollisionComponent* mCollider = nullptr;
	GoombaMove* mGoombaMovement = nullptr;
	bool mIsStomped = false;
	float mTimeAlive = 0.0f;
};