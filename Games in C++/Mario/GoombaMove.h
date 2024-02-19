#pragma once
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Math.h"

const float GOOMBA_SPEED = -100.0f;
const float GRAVITY = 2000.0f;

class GoombaMove : public MoveComponent
{
public:
	GoombaMove(class Actor* owner);

	// Update the move component
	void Update(float deltaTime) override;

	// Getters/setters
	Vector2 GetVelocity() const { return mVelocity; }
	void SetVelocity(Vector2 speed) { mVelocity = speed; }
	void SetVelocityX(float speed) { mVelocity.x = speed; }
	void SetVelocityY(float speed) { mVelocity.y = speed; }

protected:
	// Forward speed (in pixels/second)

	void UpdateVelocityFromCollision(CollSide side, Vector2 offset);

	Vector2 mVelocity;
	float mGravity;
};