#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (mAnimName.empty())
	{
		return;
	}

	if (!mIsPaused)
	{
		mAnimTimer += mAnimFPS * deltaTime;
	}

	while (mAnimTimer >= mAnims.at(mAnimName).size())
	{
		mAnimTimer -= mAnims.at(mAnimName).size();
	}
	SetTexture(mAnims.at(mAnimName)[static_cast<int>(mAnimTimer)]);
}

void AnimatedSprite::AddAnimation(const std::string& name, const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
