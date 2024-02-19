#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(class Actor* owner)
: Component(owner, 50)
, mAngularSpeed(0.0f)
, mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	float curAngle = mOwner->GetRotation() + mAngularSpeed * deltaTime;

	if (curAngle >= (Math::TwoPi))
	{
		curAngle -= Math::TwoPi;
	} //checks for overflow

	if (curAngle <= (-1 * Math::TwoPi))
	{
		curAngle += Math::TwoPi;
	} //checks for overflow

	mOwner->SetRotation(curAngle);

	Vector2 direction = mOwner->GetForward(curAngle); //update angle
	Vector2 position = mOwner->GetPosition();

	mOwner->SetPosition(position + (direction * mForwardSpeed * deltaTime));
}
