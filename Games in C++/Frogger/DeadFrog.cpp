#include "DeadFrog.h"
DeadFrog::DeadFrog(Game* game)
: Actor(game)
{
}
void DeadFrog::OnUpdate(float deltaTime)
{
	mTimeAlive -= deltaTime;
	if (mTimeAlive <= 0.0f)
	{
		delete this; //removes laser if it has been alive for too long
	}
}