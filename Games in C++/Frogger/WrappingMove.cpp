#include "WrappingMove.h"
#include "Actor.h"

WrappingMove::WrappingMove(Actor* owner)
: MoveComponent(owner)
{
}
Vector2 WrappingMove::GetDirection() const
{
	return mDirection;
}
void WrappingMove::Update(float deltaTime)
{
	Vector2 position = mOwner->GetPosition();

	position.x += mDirection.x * mForwardSpeed * deltaTime;
	if (position.x < 0)
	{
		position.x = 448;
	}
	if (position.x > 448)
	{
		position.x = 0;
	}

	mOwner->SetPosition(position);
}
void WrappingMove::SetDirection(Vector2 direction)
{
	mDirection.x = direction.x;
	mDirection.y = direction.y;
}