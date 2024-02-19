#pragma once
#include "Actor.h"

class CollisionComponent;
class SpriteComponent;
class Block : public Actor
{
public:
	Block(class Game* game, char type);
	~Block();
	const CollisionComponent* GetCollisionComponent();

private:
	//void OnUpdate(float deltaTime) override;
	SpriteComponent* mSprite = nullptr;
	CollisionComponent* mCollider = nullptr;
};