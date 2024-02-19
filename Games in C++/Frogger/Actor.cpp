#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector2::Zero)
, mScale(1.0f)
, mRotation(0.0f)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	for (int i = 0; i < mComponents.size(); i++)
	{
		delete mComponents[i];
	}
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	if (mState == ActorState::Active)
	{
		for (int i = 0; i < mComponents.size(); i++)
		{
			mComponents[i]->Update(deltaTime);
		}
	}
	OnUpdate(deltaTime);
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	if (mState == ActorState::Active)
	{
		for (int i = 0; i < mComponents.size(); i++)
		{
			mComponents[i]->ProcessInput(keyState);
		}
	}
	OnProcessInput(keyState);
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}
Vector2 Actor::GetForward(float rotation)
{
	return Vector2(cos(rotation), -1 * sin(rotation));
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
