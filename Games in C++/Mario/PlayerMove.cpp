
#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"
#include "PlayerMove.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Block.h"
#include "Goomba.h"
#include "SDL2/SDL_mixer.h"
#include "AnimatedSprite.h"

PlayerMove::PlayerMove(class Actor* owner)
: MoveComponent(owner)
, mVelocity(0.0f, 0.0f)
, mGravity(GRAVITY)
{
}

void PlayerMove::Update(float deltaTime)
{
	if (mOwner->GetState() == ActorState::Paused)
	{
		return;
	}
	Vector2 position = mOwner->GetPosition();

	mOwner->SetPosition(position + (mVelocity * deltaTime));
	mVelocity.y += mGravity * deltaTime;

	if (mDead)
	{
		return;
	}
	Game* game = mOwner->GetGame();

	if (mOwner->GetPosition().x >= FLAG_POSITION) //Level Cleared!
	{
		Mix_HaltChannel(game->GetBackgroundMusicChannel());
		Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/StageClear.wav"), 0);

		mOwner->SetState(ActorState::Paused);
	}

	bool hasNotBonked = true;
	bool noneBelow = true;
	for (size_t i = 0; i < game->GetAllBlocks().size(); i++)
	{
		Vector2 offset;
		CollSide collidCheck = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(
			game->GetAllBlocks()[i]->GetCollisionComponent(), offset);

		switch (collidCheck)
		{
		case (CollSide::None):
		{
			break;
		}
		case (CollSide::Top):
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			SetVelocityY(0.0f);
			static_cast<Player*>(mOwner)->SetAirborn(true);
			noneBelow = false;
		}
		case (CollSide::Bottom):
		{
			if (hasNotBonked && !static_cast<Player*>(mOwner)->IsAirborn())
			{
				Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/Bump.wav"), 0);
				hasNotBonked = false;
			}
			if (GetVelocity().y < 0)
			{
				SetVelocityY(0.0f);
			}
			break;
		}
		default:
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			break;
		}
		}
	}
	if (noneBelow)
	{
		static_cast<Player*>(mOwner)->SetAirborn(false);
	}

	for (size_t i = 0; i < mOwner->GetGame()->GetAllGoombas().size(); i++)
	{
		Vector2 offset;
		CollSide collidCheck = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(
			game->GetAllGoombas()[i]->GetComponent<CollisionComponent>(), offset);

		if (collidCheck != CollSide::None)
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
		switch (collidCheck)
		{
		case (CollSide::None):
		{
			break;
		}
		case (CollSide::Top):
		{
			PlayerStomp(game->GetAllGoombas()[i]);
			break;
		}
		case (CollSide::Bottom):
		{
			PlayerDied();
			return;
		}
		default:
		{
			if (!static_cast<Player*>(mOwner)->IsAirborn())
			{
				PlayerStomp(game->GetAllGoombas()[i]);
				break;
			}
			else
			{
				PlayerDied();
				return;
			}
		}
		}
	}
	if (mOwner->GetPosition().y > WINDOW_LENGTH) //where the death starts
	{
		PlayerDied();
		return;
	}
}

void PlayerMove::PlayerDied()
{
	Mix_HaltChannel(mOwner->GetGame()->GetBackgroundMusicChannel());
	Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);

	mOwner->GetComponent<AnimatedSprite>()->SetAnimation("dead");

	SetVelocity(Vector2(0.0f, 0.0f));
	mOwner->SetState(ActorState::Paused);

	static_cast<Player*>(mOwner)->StartDeath();
	mDead = true;
}

void PlayerMove::PlayerStomp(Goomba* goomba)
{
	Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);

	SetVelocityY(JUMP_SPEED / 2.0f);

	goomba->Stomp();
}