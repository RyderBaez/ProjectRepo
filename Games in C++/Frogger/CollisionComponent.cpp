#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	return (other->GetMax().x >= GetMin().x && other->GetMax().y >= GetMin().y &&
			GetMax().x >= other->GetMin().x && GetMax().y >= other->GetMin().y);
}

Vector2 CollisionComponent::GetMin() const
{

	return Vector2(GetCenter().x - ((mWidth * mOwner->GetScale()) / 2.0f),
				   GetCenter().y - ((mHeight * mOwner->GetScale()) / 2.0f));
}

Vector2 CollisionComponent::GetMax() const
{
	return Vector2(GetCenter().x + ((mWidth * mOwner->GetScale()) / 2.0f),
				   GetCenter().y + ((mHeight * mOwner->GetScale()) / 2.0f));
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector2& offset) const
{
	if (!Intersect(other))
	{
		return CollSide::None;
	}
	float minDist = Math::Infinity;
	Vector2 localMin = GetMin();
	Vector2 localMax = GetMax();
	Vector2 otherMin = other->GetMin();
	Vector2 otherMax = other->GetMax();
	CollSide intersect = CollSide::None;

	float topDist = (otherMin.y - localMax.y);
	if (minDist >= std::abs(topDist) && topDist <= 0)
	{
		minDist = std::abs(topDist);
		intersect = CollSide::Top;
		offset = Vector2(0.0f, -1 * minDist);
	}

	float rightDist = otherMax.x - localMin.x;
	if (minDist >= std::abs(rightDist) && rightDist >= 0)
	{
		minDist = std::abs(rightDist);
		intersect = CollSide::Right;
		offset = Vector2(minDist, 0.0f);
	}

	float leftDist = otherMin.x - localMax.x;
	if (minDist >= std::abs(leftDist) && leftDist <= 0)
	{
		minDist = std::abs(leftDist);
		intersect = CollSide::Left;
		offset = Vector2(-1 * minDist, 0.0f);
	}

	float bottomDist = otherMax.y - localMin.y;
	if (minDist > std::abs(bottomDist) && (bottomDist) > 0)
	{
		minDist = std::abs(bottomDist);
		intersect = CollSide::Bottom;
		offset = Vector2(0.0f, minDist);
	}

	return intersect;
}
