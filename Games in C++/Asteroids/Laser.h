#pragma once
#include "Actor.h"
const float LASER_SPEED = 400.0f;
const float LASER_DURATION = 1.0f;

class Laser : public Actor
{
public:
	Laser(class Game* game);

private:
	void OnUpdate(float deltaTime) override;

	class MoveComponent* mMove = nullptr;
	class SpriteComponent* mSprite = nullptr;

	float mTimeAlive = 1.0f;
};