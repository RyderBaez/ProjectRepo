#include <SDL2/SDL_image.h>
#include "Laser.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Random.h"
#include "Asteroid.h"

Laser::Laser(Game* game)
: Actor(game)
{
	mMove = new MoveComponent(this);
	mSprite = new SpriteComponent(this);

	mSprite->SetTexture(mGame->GetTexture("Assets/Laser.png"));
	mMove->SetForwardSpeed(LASER_SPEED);
}
void Laser::OnUpdate(float deltaTime)
{
	for (size_t i = 0; i < mGame->GetAsteroids().size(); i++)
	{
		if (Vector2::Distance(mPosition, mGame->GetAsteroids()[i]->GetPosition()) <= ASTEROID_SIZE)
		{
			SetState(ActorState::Destroy);
			mGame->GetAsteroids()[i]->SetState(ActorState::Destroy);
			return;
		}
	}

	mTimeAlive -= deltaTime;
	if (mTimeAlive <= 0.0f)
	{
		delete this; //removes laser if it has been alive for too long
	}
}