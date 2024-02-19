
#include "Player.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "Block.h"
#include "Goomba.h"
#include "SDL2/SDL_mixer.h"

Player::Player(Game* game)
: Actor(game)
{
	mSprite = new AnimatedSprite(this, 200);

	mPlayerMovement = new PlayerMove(this);

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(TILE_SIZE, TILE_SIZE);

	ConstructAnimations();
}
Player::~Player()
{
}

void Player::OnUpdate(float deltaTime)
{
	if (mPauseBeforeDeath > 0.0f && !mPauseOver) //special update code for unique death animation
	{
		mPauseBeforeDeath -= deltaTime;
		return;
	}
	if (mPauseBeforeDeath <= 0.0f && !mPauseOver)
	{
		mPauseOver = true;
		mState = ActorState::Active;
		mPlayerMovement->SetVelocityY(JUMP_SPEED);
		return;
	}
	if (mState == ActorState::Paused)
	{
		return;
	}

	if (mPosition.y > WINDOW_LENGTH + TILE_SIZE / 2.0f && mIsDead)
	{
		mState = ActorState::Paused;
		return;
	}
	if (mIsDead)
	{
		return;
	}

	if (!mGrounded)
	{
		if (mPlayerMovement->GetVelocity().x > 0)
		{
			mSprite->SetAnimation("jumpRight");
		}
		else if (mPlayerMovement->GetVelocity().x < 0)
		{
			mSprite->SetAnimation("jumpLeft");
		}
	}
	else
	{
		if (mPlayerMovement->GetVelocity().x > 0)
		{
			mSprite->SetAnimation("runRight");
		}
		else if (mPlayerMovement->GetVelocity().x < 0)
		{
			mSprite->SetAnimation("runLeft");
		}
		else
		{
			mSprite->SetAnimation("idle");
		}
	}
	if (mPosition.x - SCREEN_OFFSET > mGame->GetCameraPos().x)
	{
		mGame->GetCameraPos().x = mPosition.x - SCREEN_OFFSET;
	}
	if (mPosition.x < mGame->GetCameraPos().x)
	{
		mPosition.x = mGame->GetCameraPos().x;
	}
	if (mGame->GetCameraPos().x < 0)
	{
		mGame->GetCameraPos().x = 0;
	}
}
void Player::OnProcessInput(const Uint8* keyState)
{
	if (mIsDead)
	{
		return;
	}
	float newVelocityX = 0.0f;
	if (keyState[SDL_SCANCODE_D] != 0)
	{
		newVelocityX += MARIO_SPEED;
	}
	if (keyState[SDL_SCANCODE_A] != 0)
	{
		newVelocityX -= MARIO_SPEED;
	}
	if (keyState[SDL_SCANCODE_SPACE] != 0 && !mSpacePressed && mGrounded) //jump
	{
		mGrounded = false;
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Jump.wav"), 0);
		mPlayerMovement->SetVelocityY(JUMP_SPEED);
		if (mPlayerMovement->GetVelocity().x >= 0)
		{
			mSprite->SetAnimation("jumpRight");
		}
		else if (mPlayerMovement->GetVelocity().x < 0)
		{
			mSprite->SetAnimation("jumpLeft");
		}
	}
	mSpacePressed = (keyState[SDL_SCANCODE_SPACE] != 0);
	mPlayerMovement->SetVelocityX(newVelocityX);
}

void Player::ConstructAnimations()
{
	std::vector<SDL_Texture*> runLeftAnim = {GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
											 GetGame()->GetTexture("Assets/Mario/runLeft1.png"),
											 GetGame()->GetTexture("Assets/Mario/runLeft2.png")};
	mSprite->AddAnimation("runLeft", runLeftAnim);

	std::vector<SDL_Texture*> runRightAnim = {GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
											  GetGame()->GetTexture("Assets/Mario/runRight1.png"),
											  GetGame()->GetTexture("Assets/Mario/runRight2.png")};
	mSprite->AddAnimation("runRight", runRightAnim);

	std::vector<SDL_Texture*> jumpLeftAnim = {GetGame()->GetTexture("Assets/Mario/JumpLeft.png")};
	mSprite->AddAnimation("jumpLeft", jumpLeftAnim);

	std::vector<SDL_Texture*> jumpRightAnim = {GetGame()->GetTexture("Assets/Mario/JumpRight.png")};
	mSprite->AddAnimation("jumpRight", jumpRightAnim);

	std::vector<SDL_Texture*> deathAnim = {GetGame()->GetTexture("Assets/Mario/Dead.png")};
	mSprite->AddAnimation("dead", deathAnim);

	std::vector<SDL_Texture*> idleAnim = {GetGame()->GetTexture("Assets/Mario/Idle.png")};
	mSprite->AddAnimation("idle", idleAnim);

	mSprite->SetAnimation("idle");
}

const bool Player::IsAirborn() const
{
	return mGrounded;
}
void Player::SetAirborn(bool status)
{
	mGrounded = status;
}
void Player::StartDeath()
{
	mIsDead = true;
	mPauseOver = false;
	mPauseBeforeDeath = DEATH_SPRITE;
}