#include <SDL2/SDL_image.h>
#include "Asteroid.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Random.h"
Asteroid::Asteroid(Game* game)
: Actor(game)
{
	mMove = new MoveComponent(this);
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture((mGame)->GetTexture("Assets/Asteroid.png"));

	mMove->SetForwardSpeed(ASTEROID_SPEED);

	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi)); //gives random location and rotation
	SetPosition(Random::GetVector(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_LENGTH)));

	mGame->AddAsteroid(this);
}
Asteroid::~Asteroid()
{
	mGame->RemoveAsteroid(this);
}
void Asteroid::OnUpdate(float deltaTime)
{
	if (mPosition.x < 0.0f) //checks for going out of bounds
	{
		mPosition.x = static_cast<float>(WINDOW_WIDTH);
	}

	if (mPosition.x > WINDOW_WIDTH)
	{
		mPosition.x = 0.0f;
	}

	if (mPosition.y < 0.0f)
	{
		mPosition.y = static_cast<float>(WINDOW_LENGTH);
	}

	if (mPosition.y > WINDOW_LENGTH)
	{
		mPosition.y = 0.0f;
	}
}