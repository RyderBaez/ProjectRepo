#include "Vehicle.h"
#include "WrappingMove.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Math.h"
#include <iostream>

Vehicle::Vehicle(Game* game, char type, float direction)
: Actor(game)
{
	mGame->AddVehicle(this);

	mMove = new WrappingMove(this);
	mMove->SetDirection(Vector2(direction, 0.0f));
	mMove->SetForwardSpeed(VEHICLE_SPEED);

	mCollider = new CollisionComponent(this);
	mSprite = new SpriteComponent(this);

	if (type == 'T')
	{
		std::string blockTexture = "Assets/Truck.png";

		mSprite->SetTexture((this->mGame)->GetTexture(blockTexture));

		mCollider->SetSize(TRUCK_WIDTH, TRUCK_LENGTH);
	}
	else
	{

		std::string blockTexture = "Assets/Car";
		blockTexture += type;
		blockTexture += ".png";

		mSprite->SetTexture((this->mGame)->GetTexture(blockTexture));

		mCollider->SetSize(TILE_SIZE, TILE_SIZE);
	}
}

Vehicle::~Vehicle()
{
	mGame->RemoveVehicle(this);
}

CollisionComponent* Vehicle::GetCollisionComponent()
{
	return mCollider;
}

void Vehicle::OnUpdate(float deltaTime)
{

	Vector2 frogPosition = mGame->GetFrogPosition();
	Vector2 distance = frogPosition - this->GetPosition();

	mMove = GetComponent<WrappingMove>();
	if (mMove != nullptr)
	{

		float dotProduct = Vector2::Dot(distance, mMove->GetDirection());
		if (Math::Acos(dotProduct / distance.Length()) <= (Math::Pi / 6.0f) && dotProduct >= 0)
		{
			mMove->SetForwardSpeed(VEHICLE_SPEED / 2.0f); // slowed down by half
		}
		else
		{
			mMove->SetForwardSpeed(VEHICLE_SPEED);
		}
	}
}
