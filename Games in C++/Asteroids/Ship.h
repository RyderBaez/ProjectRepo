#pragma once
#include "Actor.h"
const float SHIP_SPEED = 300.0f;
const float SHIP_ANGLE_SPEED = 3.0f;

class Ship : public Actor
{
public:
	Ship(class Game* game);

private:
	class MoveComponent* mMove = nullptr;
	class SpriteComponent* mSprite = nullptr;

	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;

	float mLaserCooldown = 0.0f;
};