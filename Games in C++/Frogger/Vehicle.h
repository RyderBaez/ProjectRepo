#pragma once
#include "Actor.h"
#include <string>
const float VEHICLE_SPEED = 50.0f;

const int TRUCK_WIDTH = 64;
const int TRUCK_LENGTH = 24;

class Vehicle : public Actor
{
public:
	Vehicle(class Game* game, char type, float direction);
	~Vehicle();

	class CollisionComponent* GetCollisionComponent();

private:
	//void OnUpdate(float deltaTime) override;
	void OnUpdate(float deltaTime) override;

	class WrappingMove* mMove = nullptr;
	class SpriteComponent* mSprite = nullptr;
	class CollisionComponent* mCollider = nullptr;
};