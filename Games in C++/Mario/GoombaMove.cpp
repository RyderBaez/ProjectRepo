
#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"
#include "GoombaMove.h"
#include "Game.h"
#include "Goomba.h"
#include "Block.h"

GoombaMove::GoombaMove(class Actor* owner)
: MoveComponent(owner)
, mVelocity(GOOMBA_SPEED, 0.0f)
, mGravity(GRAVITY)
{
}

void GoombaMove::Update(float deltaTime)
{

	Vector2 position = mOwner->GetPosition();
	mOwner->SetPosition(position + (mVelocity * deltaTime));
	mVelocity.y += mGravity * deltaTime;

	for (size_t i = 0; i < mOwner->GetGame()->GetAllBlocks().size(); i++)
	{
		Vector2 offset;
		CollSide collidCheck = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(
			mOwner->GetGame()->GetAllBlocks()[i]->GetCollisionComponent(), offset);

		UpdateVelocityFromCollision(collidCheck, offset);
	}

	for (size_t i = 0; i < mOwner->GetGame()->GetAllGoombas().size(); i++)
	{
		if (mOwner->GetGame()->GetAllGoombas()[i] != mOwner)
		{
			Vector2 offset;
			CollSide collidCheck = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(
				mOwner->GetGame()->GetAllGoombas()[i]->GetComponent<CollisionComponent>(), offset);

			UpdateVelocityFromCollision(collidCheck, offset);
		}
	}
}

void GoombaMove::UpdateVelocityFromCollision(CollSide side, Vector2 offset)
{
	if (side != CollSide::None)
	{
		mOwner->SetPosition(mOwner->GetPosition() + offset);
		switch (side)
		{

		case (CollSide::Top):
		{
			SetVelocityY(0.0f);
			break;
		}
		case (CollSide::Left):
		{
			SetVelocityX(GetVelocity().x * -1);
			break;
		}
		case (CollSide::Right):
		{
			SetVelocityX(GetVelocity().x * -1);
			break;
		}
		default:
		{
			break;
		}
		}
	}
}