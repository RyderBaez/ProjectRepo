#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"

GhostAI::GhostAI(class Actor* owner)
: Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
	mTime += deltaTime;
	mGhost->SetPosition(mGhost->GetPosition() + mDirection * mStateSpeed * deltaTime);

	if (mGhost->GetComponent<CollisionComponent>()->Intersect(
			mNextNode->GetComponent<CollisionComponent>()))
	{
		mGhost->SetPosition(mNextNode->GetPosition());

		ChangeState(mNextNode);

		SetNewTarget();

		FindNextNode(false);
	}
}

void GhostAI::Frighten()
{
	if (mState != Dead)
	{

		if (mPrevNode != nullptr && mState != Frightened)
		{
			PathNode* temp = mPrevNode;
			mPrevNode = mNextNode;
			mNextNode = temp;
			mDirection *= -1;
		}
		mTime = 0.0f;
		mState = Frightened;
		mStateSpeed = FRIGHTENED_SPEED;
		mTargetNode = nullptr;

		mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
	}
}

void GhostAI::Start(PathNode* startNode)
{
	mTime = 0.0f;
	mGhost->SetPosition(startNode->GetPosition());
	mState = Scatter;
	mStateSpeed = DEFAULT_SPEED;
	mPrevNode = nullptr;
	mNextNode = startNode;
}

void GhostAI::Die()
{
	mTime = 0.0f;
	mState = Dead;
	mStateSpeed = DEAD_SPEED;
	if (mGhost->GetComponent<CollisionComponent>()->Intersect(
			mNextNode->GetComponent<CollisionComponent>()))
	{
		mGhost->SetPosition(mNextNode->GetPosition());

		ChangeState(mNextNode);

		SetNewTarget();

		FindNextNode(false);
	}
	else
	{

		//mNextNode = mPrevNode;

		SetTexture();
	}

	mTargetNode = mOwner->GetGame()->GetGhostPen();
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render, static_cast<int>(mOwner->GetPosition().x),
						   static_cast<int>(mOwner->GetPosition().y),
						   static_cast<int>(mNextNode->GetPosition().x),
						   static_cast<int>(mNextNode->GetPosition().y));
	}
}

void GhostAI::ChangeState(PathNode* node)
{
	if (mState == Scatter)
	{
		mStateSpeed = DEFAULT_SPEED;
	}
	if (mState == Frightened && mTime >= 5.0f)
	{
		mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
	}
	if (mState == Frightened && mTime >= 7.0f)
	{
		mState = Scatter;
		mTime = 0.0f;
		mStateSpeed = DEFAULT_SPEED;
	}
	if (mState == Scatter && mTime >= 5.0f)
	{
		mState = Chase;
		mTime = 0.0f;
		mStateSpeed = DEFAULT_SPEED;
	}
	if (mState == Chase && mTime >= 20.0f)
	{
		mState = Scatter;
		mTime = 0.0f;
		mStateSpeed = DEFAULT_SPEED;
	}
	if (mState == Dead && node->GetType() == PathNode::Type::Ghost)
	{
		mState = Scatter;
		mTime = 0.0f;
		mStateSpeed = DEFAULT_SPEED;
	}
}

void GhostAI::SetNewTarget()
{
	switch (mState)
	{

	case (Frightened):
	{
		PickRandomTarget(0);
		break;
	}
	case (Scatter):
	{
		mTargetNode = mGhost->GetScatterNode();
		break;
	}
	case (Chase):
	{
		switch (mGhost->GetType())
		{

		case (Ghost::Blinky):
		{
			BlinkyMove();
			break;
		}
		case (Ghost::Pinky):
		{
			PinkyMove();
			break;
		}
		case (Ghost::Inky):
		{
			InkyMove();
			break;
		}
		case (Ghost::Clyde):
		{
			ClydeMove();
			break;
		}
		}
		break;
	}
	case (Dead):
	{
		mTargetNode = mOwner->GetGame()->GetGhostPen();
		break;
	}
	}
}

void GhostAI::FindNextNode(bool useGhost)
{
	if (mTargetNode == nullptr)
	{
		SetNewTarget();
	}
	float shortestDistance = Math::Infinity;
	PathNode* nextDest = mPrevNode;
	for (size_t i = 0; i < mNextNode->mAdjacent.size(); i++)
	{
		if (mNextNode->mAdjacent[i] == mPrevNode)
		{
			continue;
		}
		if (mNextNode->mAdjacent[i]->GetType() == PathNode::Ghost && (!useGhost && mState != Dead))
		{
			continue;
		}
		if (mNextNode->mAdjacent[i]->GetType() == PathNode::Tunnel)
		{
			continue;
		}
		if (mTargetNode == nullptr)
		{
			return;
		}
		float curDist = Vector2::Distance(mNextNode->mAdjacent[i]->GetPosition(),
										  mTargetNode->GetPosition());

		if (curDist < shortestDistance)
		{
			nextDest = mNextNode->mAdjacent[i];
			shortestDistance = curDist;
		}
	}
	if (nextDest == nullptr)
	{
		FindNextNode(true);
		return;
	}
	mPrevNode = mNextNode;
	mNextNode = nextDest;

	mDirection = Vector2::Normalize((mNextNode->GetPosition() - mPrevNode->GetPosition()));
	SetTexture();
}

void GhostAI::PickRandomTarget(int resets)
{
	int randMove = Random::GetIntRange(1, static_cast<int>(mNextNode->mAdjacent.size()));
	for (size_t i = 0; i < mNextNode->mAdjacent.size(); i++)
	{
		randMove--;
		if (mNextNode->mAdjacent[i] == mPrevNode)
		{
			continue;
		}
		if (mNextNode->mAdjacent[i]->GetType() == PathNode::Ghost && (resets <= 0))
		{
			continue;
		}
		if (mNextNode->mAdjacent[i]->GetType() == PathNode::Tunnel && resets <= 1)
		{
			continue;
		}

		mTargetNode = mNextNode->mAdjacent[i];

		if (randMove == 0)
		{
			break;
		}
	}
	if (mTargetNode == nullptr)
	{
		PickRandomTarget(resets + 1);
	}
}

void GhostAI::BlinkyMove()
{
	mTargetNode = mOwner->GetGame()->GetPlayer()->GetPrevNode();

	if (mTargetNode->GetType() == PathNode::Tunnel)
	{
		mTargetNode = ClosestNode(mTargetNode->GetPosition());
	}
}

void GhostAI::PinkyMove()
{
	mTargetNode = ClosestNode((mOwner->GetGame()->GetPlayer()->GetPointInFrontOf(PINKY_OFFSET)));
}

void GhostAI::InkyMove()
{
	Vector2 targetArea = mOwner->GetGame()->GetPlayer()->GetPointInFrontOf(INKY_OFFSET) -
						 mOwner->GetGame()->GetGhosts()[0]->GetPosition();

	targetArea = 2 * targetArea + mOwner->GetGame()->GetGhosts()[0]->GetPosition();

	mTargetNode = ClosestNode(targetArea);
}

void GhostAI::ClydeMove()
{
	if (Vector2::Distance(mGhost->GetPosition(), mOwner->GetGame()->GetPlayer()->GetPosition()) >
		CLYDE_SCARE)
	{
		BlinkyMove();
	}
	else
	{
		mTargetNode = mGhost->GetScatterNode();
	}
}

PathNode* GhostAI::ClosestNode(Vector2 position)
{
	std::vector<PathNode*> allNodes = mOwner->GetGame()->GetPathNodes();

	float shortestDistance = Math::Infinity;
	PathNode* closest = nullptr;

	for (size_t i = 0; i < allNodes.size(); i++)
	{

		if (allNodes[i]->GetType() == PathNode::Tunnel)
		{
			continue;
		}
		if (allNodes[i]->GetType() == PathNode::Ghost)
		{
			continue;
		}

		float curDist = Vector2::Distance(position, allNodes[i]->GetPosition());

		if (curDist < shortestDistance)
		{
			closest = allNodes[i];
			shortestDistance = curDist;
		}
	}
	return closest;
}
void GhostAI::SetTexture()
{
	if (mState != Frightened)
	{

		if (mDirection.x > 0)
		{
			mGhost->GetComponent<AnimatedSprite>()->SetAnimation("right");
			if (mState == Dead)
			{
				mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadright");
			}
		}

		if (mDirection.x < 0)
		{
			mGhost->GetComponent<AnimatedSprite>()->SetAnimation("left");
			if (mState == Dead)
			{
				mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadleft");
			}
		}

		if (mDirection.y > 0)
		{
			mGhost->GetComponent<AnimatedSprite>()->SetAnimation("down");
			if (mState == Dead)
			{
				mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deaddown");
			}
		}

		if (mDirection.y < 0)
		{
			mGhost->GetComponent<AnimatedSprite>()->SetAnimation("up");
			if (mState == Dead)
			{
				mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadup");
			}
		}
	}
}