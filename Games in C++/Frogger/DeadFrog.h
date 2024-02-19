#pragma once
#include "Actor.h"
class DeadFrog : public Actor
{
public:
	DeadFrog(class Game* game);

private:
	void OnUpdate(float deltaTime) override;

	class SpriteComponent* mSprite = nullptr;

	float mTimeAlive = 0.5f;
};