#include <SDL2/SDL_image.h>
#include "Ship.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
: Actor(game)
{
	mMove = new MoveComponent(this);
	mSprite = new SpriteComponent(this);

	mSprite->SetTexture((this->mGame)->GetTexture("Assets/Ship.png"));
}
void Ship::OnProcessInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] != 0 && mLaserCooldown <= 0.0f)
	{
		Laser* laser = new Laser(this->mGame);
		SpriteComponent* laserSprite = new SpriteComponent(laser);
		laserSprite->SetTexture((this->mGame)->GetTexture("Assets/Laser.png"));

		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		mLaserCooldown = LASER_DURATION;
	}

	if (mLaserCooldown <= 0.0f) //covering for potential float overflow
	{
		mLaserCooldown = 0.0f;
	}

	float newForwardSpeed = 0.0f;
	if (keyState[SDL_SCANCODE_W] != 0)
	{
		newForwardSpeed += SHIP_SPEED;
	}
	if (keyState[SDL_SCANCODE_S] != 0)
	{
		newForwardSpeed -= SHIP_SPEED;
	}

	mMove->SetForwardSpeed(newForwardSpeed); //set updated value

	float newAngularSpeed = 0.0f;
	if (keyState[SDL_SCANCODE_A] != 0)
	{
		newAngularSpeed += SHIP_ANGLE_SPEED;
	}
	if (keyState[SDL_SCANCODE_D] != 0)
	{
		newAngularSpeed -= SHIP_ANGLE_SPEED;
	}
	mMove->SetAngularSpeed(newAngularSpeed);

	if (mMove->GetForwardSpeed() != 0)
	{
		mSprite->SetTexture((this->mGame)->GetTexture("Assets/ShipThrust.png"));
	}
	else
	{
		mSprite->SetTexture((this->mGame)->GetTexture("Assets/Ship.png"));
	}
}
void Ship::OnUpdate(float deltaTime)
{
	mLaserCooldown -= deltaTime;
}