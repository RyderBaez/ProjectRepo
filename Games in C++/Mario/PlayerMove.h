#pragma once
#include "MoveComponent.h"
#include "Math.h"

const float GRAVITY = 2000.0f;
class Goomba;

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Actor* owner);

	// Update the move component
	void Update(float deltaTime) override;

	// Getters/setters
	Vector2 GetVelocity() const { return mVelocity; }
	void SetVelocity(Vector2 speed) { mVelocity = speed; }
	void SetVelocityX(float speed) { mVelocity.x = speed; }
	void SetVelocityY(float speed) { mVelocity.y = speed; }

protected:
	// Forward speed (in pixels/second)
	Vector2 mVelocity;
	float mGravity;
	bool mDead = false;
	void PlayerDied();
	void PlayerStomp(Goomba* goomba);
};