#pragma once
#include "Actor.h"
const float ASTEROID_SPEED = 150.0f;
const float ASTEROID_SIZE = 70.0f;
class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	~Asteroid();

private:
	void OnUpdate(float deltaTime) override;
	class MoveComponent* mMove = nullptr;
	class SpriteComponent* mSprite = nullptr;
};