#include "Spawner.h"
#include "Game.h"
#include "Goomba.h"
#include "Player.h"
#include "Math.h"

Spawner::Spawner(class Game* game)
: Actor(game)
{
}
Spawner::~Spawner()
{
}
void Spawner::OnUpdate(float deltaTime)
{
	Vector2 offset = mPosition - mGame->GetPlayer()->GetPosition();
	if (offset.Length() < WINDOW_WIDTH)
	{
		Goomba* goomba = new Goomba(mGame);
		goomba->SetPosition(mPosition);
		mState = ActorState::Destroy;
	}
}