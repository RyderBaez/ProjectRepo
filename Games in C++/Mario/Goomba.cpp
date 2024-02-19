
#include "Goomba.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "GoombaMove.h"
#include "Block.h"
#include "AnimatedSprite.h"

Goomba::Goomba(Game* game)
: Actor(game)
{
	mGame->AddGoomba(this);

	std::vector<SDL_Texture*> walkAnim = {GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
										  GetGame()->GetTexture("Assets/Goomba/Walk1.png")};

	std::vector<SDL_Texture*> deathAnim = {GetGame()->GetTexture("Assets/Goomba/Dead.png")};

	mSprite = new AnimatedSprite(this, 120);
	mSprite->AddAnimation("walk", walkAnim);
	mSprite->AddAnimation("dead", deathAnim);
	mSprite->SetAnimation("walk");

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(TILE_SIZE, TILE_SIZE);

	mGoombaMovement = new GoombaMove(this);
}
Goomba::~Goomba()
{
}

void Goomba::OnUpdate(float deltaTime)
{
	if (mIsStomped)
	{

		if (mTimeAlive == 0.0f)
		{
			mTimeAlive = DEATH_SPRITE;
			mGame->RemoveGoomba(this);
			mSprite->SetAnimation("dead");
			mGoombaMovement->SetVelocityX(0.0f);
			return;
		}

		mTimeAlive -= deltaTime;
		if (mTimeAlive <= 0.0f)
		{
			delete this;
			return;
		}
	}
	if (mPosition.y > WINDOW_LENGTH + TILE_SIZE)
	{
		mGame->RemoveGoomba(this);
		delete this;
		return;
	}
}
void Goomba::Stomp()
{
	mIsStomped = true;
}
