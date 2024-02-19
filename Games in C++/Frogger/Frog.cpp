#include <SDL2/SDL_image.h>
#include "Frog.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Math.h"
#include <utility>
#include "Vehicle.h"
#include "DeadFrog.h"
#include "Log.h"
#include "WrappingMove.h"

Frog::Frog(Game* game)
: Actor(game)
{
	mSprite = new SpriteComponent(this, 120);

	mSprite->SetTexture((this->mGame)->GetTexture("Assets/Frog.png"));

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(FROG_SIZE, FROG_SIZE);

	mPrevInputs.insert(std::make_pair(SDL_SCANCODE_W, false));
	mPrevInputs.insert(std::make_pair(SDL_SCANCODE_A, false));
	mPrevInputs.insert(std::make_pair(SDL_SCANCODE_S, false));
	mPrevInputs.insert(std::make_pair(SDL_SCANCODE_D, false));
}

Frog::~Frog()
{
}

void Frog::OnProcessInput(const Uint8* keyState)
{
	if (mState == ActorState::Paused)
	{
		return;
	}

	if (!mPrevInputs.at(SDL_SCANCODE_W) && keyState[SDL_SCANCODE_W] != 0)
	{
		this->mPosition.y -= MOVE_AMOUNT;
	}

	if (!mPrevInputs.at(SDL_SCANCODE_A) && keyState[SDL_SCANCODE_A] != 0)
	{
		this->mPosition.x -= MOVE_AMOUNT;
	}

	if (!mPrevInputs.at(SDL_SCANCODE_S) && keyState[SDL_SCANCODE_S] != 0)
	{
		this->mPosition.y += MOVE_AMOUNT;
	}

	if (!mPrevInputs.at(SDL_SCANCODE_D) && keyState[SDL_SCANCODE_D] != 0)
	{
		this->mPosition.x += MOVE_AMOUNT;
	}

	GetButtonState(keyState, SDL_SCANCODE_W);
	GetButtonState(keyState, SDL_SCANCODE_A);
	GetButtonState(keyState, SDL_SCANCODE_S);
	GetButtonState(keyState, SDL_SCANCODE_D);

	mPosition.x = Math::Clamp<float>(mPosition.x, 0.0f, static_cast<float>(WINDOW_WIDTH));
	mPosition.y = Math::Clamp<float>(mPosition.y, FROGGER_MIN,
									 static_cast<float>(WINDOW_LENGTH) - FROGGER_MAX_OFFSET);
}

void Frog::GetButtonState(const Uint8* keyState, int button)
{
	if (keyState[button] != 0)
	{
		mPrevInputs.at(button) = true;
	}
	else
	{
		mPrevInputs.at(button) = false;
	}
}

void Frog::OnUpdate(float deltaTime)
{

	if (mCollider->Intersect(mGame->GetGoal()->GetComponent<CollisionComponent>()))
	{
		mPosition = mGame->GetGoal()->GetPosition();
		this->SetState(ActorState::Paused);
		return;
	}

	bool onLog = false;
	for (int i = 0; i < mGame->GetAllVehicles().size(); i++)
	{
		if (mCollider->Intersect(mGame->GetAllVehicles()[i]->GetCollisionComponent()))
		{
			Actor* deathSprite = new DeadFrog(this->mGame);
			SpriteComponent* frogDeathSprite = new SpriteComponent(deathSprite);
			frogDeathSprite->SetTexture((this->mGame)->GetTexture("Assets/Dead.png"));

			deathSprite->SetPosition(GetPosition());
			SetPosition(Vector2(FROG_DEFAULT_X, FROG_DEFAULT_Y));
		}
	}

	for (int i = 0; i < mGame->GetAllLogs().size(); i++)
	{
		Vector2 offset;
		CollSide collidCheck =
			mCollider->GetMinOverlap(mGame->GetAllLogs()[i]->GetCollisionComponent(), offset);

		if (collidCheck != CollSide::None)
		{
			onLog = true;
			mPosition.x += mGame->GetAllLogs()[i]->GetMovementComponent()->GetDirection().x *
						   LOG_SPEED * deltaTime;

			if (collidCheck == CollSide::Left)
			{
				mPosition.x += offset.x + TILE_SIZE / 2.0f;
			}

			if (collidCheck == CollSide::Right)
			{
				mPosition.x += offset.x - TILE_SIZE / 2.0f;
			}
		}
	}

	if (!onLog && mPosition.y <= WATER_LEVEL)
	{
		Actor* deathSprite = new DeadFrog(this->mGame);
		SpriteComponent* frogDeathSprite = new SpriteComponent(deathSprite);
		frogDeathSprite->SetTexture((this->mGame)->GetTexture("Assets/Dead.png"));

		deathSprite->SetPosition(GetPosition());
		SetPosition(Vector2(FROG_DEFAULT_X, FROG_DEFAULT_Y));
	}
}