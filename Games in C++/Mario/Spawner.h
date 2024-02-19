#pragma once
#include "Actor.h"
class Spawner : public Actor
{
public:
	Spawner(class Game* game);
	~Spawner();

private:
	void OnUpdate(float deltaTime) override;
};