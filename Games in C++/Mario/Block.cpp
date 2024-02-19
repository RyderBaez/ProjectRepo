#include "Block.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include <string>

Block::Block(Game* game, char type)
: Actor(game)
{
	mGame->AddBlock(this);
	mSprite = new SpriteComponent(this);

	std::string blockTexture = "Assets/Block";
	blockTexture += type;
	blockTexture += ".png";

	mSprite->SetTexture((this->mGame)->GetTexture(blockTexture));

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(TILE_SIZE, TILE_SIZE);
}

Block::~Block()
{
	mGame->RemoveBlock(this);
}

const CollisionComponent* Block::GetCollisionComponent()
{
	return mCollider;
}
