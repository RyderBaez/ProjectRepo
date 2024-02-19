#include "Log.h"
#include "WrappingMove.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CollisionComponent.h"

Log::Log(Game* game, char type, float direction)
: Actor(game)
{
	mGame->AddLog(this);

	mMove = new WrappingMove(this);
	mMove->SetDirection(Vector2(direction, 0.0f));
	mMove->SetForwardSpeed(LOG_SPEED);

	mCollider = new CollisionComponent(this);
	mSprite = new SpriteComponent(this);

	std::string blockTexture = "Assets/Log";
	blockTexture += type;
	blockTexture += ".png";

	mSprite->SetTexture((this->mGame)->GetTexture(blockTexture));

	if (type == 'X')
	{
		mCollider->SetSize(LOG_SHORT, TILE_SIZE);
	}
	else if (type == 'Y')
	{
		mCollider->SetSize(LOG_MEDIUM, TILE_SIZE);
	}
	else
	{
		mCollider->SetSize(LOG_LONG, TILE_SIZE);
	}
}

Log::~Log()
{
	mGame->RemoveLog(this);
}

CollisionComponent* Log::GetCollisionComponent()
{
	if (mCollider == nullptr)
	{
		mCollider = GetComponent<CollisionComponent>();
	}
	return mCollider;
}

WrappingMove* Log::GetMovementComponent()
{
	if (mMove == nullptr)
	{
		mMove = GetComponent<WrappingMove>();
	}
	return mMove;
}