#pragma once
#include "Actor.h"

const float MARIO_SPEED = 300.0f;
const float JUMP_SPEED = -700.0f;
class AnimatedSprite;
class CollisionComponent;
class PlayerMove;

class Player : public Actor
{
public:
	Player(class Game* game);
	~Player();

	const bool IsAirborn() const;
	void SetAirborn(bool status);
	void StartDeath();

private:
	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState) override;
	void ConstructAnimations();
	//class MoveComponent* mMove = nullptr;
	AnimatedSprite* mSprite = nullptr;
	CollisionComponent* mCollider = nullptr;
	PlayerMove* mPlayerMovement = nullptr;
	bool mGrounded = false;
	bool mSpacePressed = false;
	bool mIsDead = false;
	bool mPauseOver = true;
	float mPauseBeforeDeath = 0.0f;
};