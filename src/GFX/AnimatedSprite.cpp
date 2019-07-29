#include "GFX/AnimatedSprite.hpp"

namespace GFX
{

AnimatedSprite::AnimatedSprite()
	: mCurrentAnimation(nullptr),
	  mCurrentAnimationName(""),
	  mCurrentFrame(0),
	  DEFAULT_ANIMATION({ .frames = { 0 }, .speed = sf::seconds(-1) })
{
	mClock.restart();
}

void AnimatedSprite::init(sf::Texture* texture, sf::Vector2f size)
{
	mTexture   = texture;
	mFrameSize = size;

	// Init the sprite
	mSprite.setTexture(*mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y));

	// Calculate the texture grid size.
	mGridSize = {
		(int)mTexture->getSize().x / (int)mFrameSize.x,
		(int)mTexture->getSize().y / (int)mFrameSize.y
	};
}

void AnimatedSprite::update()
{
	if (!mRunning || getCurrentAnimation()->speed.asSeconds() == -1)
		return;

	// If the clock is ready to update...
	if (mClock.getElapsedTime() > getCurrentAnimation()->speed)
	{
		// Increment the current frame
		mCurrentFrame++;
		// If it's at or above the size of the frames of this animation,
		// loop it back to 0.
		if (mCurrentFrame >= getCurrentAnimation()->frames.size())
		{
			mCurrentFrame = 0;
		}

		mClock.restart();

		// Set the sprite's rect.
		mSprite.setTextureRect(getAnimationRect());
	}
}

void AnimatedSprite::addAnimation(std::string name, AnimatedSprite::Animation animation)
{
	mAnimations[name] = animation;
}

void AnimatedSprite::removeAnimation(std::string name)
{
	mAnimations.erase(name);
}

void AnimatedSprite::setAnimation(std::string name)
{
	// Return if the animation is already running.
	if (name == mCurrentAnimationName)
	{
		return;
	}
	// Set the animation name
	mCurrentAnimationName = name;
	// Get the animation
	mCurrentAnimation = &mAnimations[name];
	// Reset the current frame & the clock.
	mClock.restart();
	mCurrentFrame = 0;
}

void AnimatedSprite::start()
{
	mCurrentFrame = 0;
	mClock.restart();
	mRunning = true;
}

void AnimatedSprite::stop()
{
	mRunning = false;
}

sf::IntRect AnimatedSprite::getAnimationRect()
{
	int id = getCurrentAnimation()->frames[mCurrentFrame];
	// X/Y pos of the frame in the texture, in units of frames.
	int x, y;
	x = id % mGridSize.x;
	y = id / mGridSize.y;
	// Now get them in pixel size.
	x *= mFrameSize.x;
	y *= mFrameSize.y;
	// Return the IntRect
	return sf::IntRect(x, y, mFrameSize.x, mFrameSize.y);
}

AnimatedSprite::Animation* AnimatedSprite::getCurrentAnimation()
{
	if (mAnimations.size() == 0 ||
		mCurrentAnimation == nullptr ||
		mCurrentAnimationName == "")
	{
		return &DEFAULT_ANIMATION;
	}
	else
	{
		return mCurrentAnimation;
	}
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

}