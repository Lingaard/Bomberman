#include "Animation.h"



Animation::Animation()
{
	mCurrentKeyFrame = sf::Vector2i(0, 0);
	mKeyFrameSize = sf::Vector2i(32, 32);
	mSpriteSheetWidth = 1;
	mAnimationSpeed = 0.2f;
	mKeyFrameDuration = 0.0f;
	mSheetDirection = 1;
}


Animation::~Animation()
{
}

void Animation::setSpriteSheetWidth(int width)
{
	mSpriteSheetWidth = width;
}

void Animation::setAnimationSpeed(float speed)
{
	mAnimationSpeed = speed;
}

void Animation::setKeyFrameDuration(float duration)
{
	mKeyFrameDuration = duration;
}

void Animation::setCurrentKeyFrameY(int y)
{
	mCurrentKeyFrame.y = y;
}

void Animation::setCurrentKeyFrameX(int x)
{
	mCurrentKeyFrame.x = x;
}

void Animation::increaseCurrentKeyFrameX()
{
	mCurrentKeyFrame.x++;
}

void Animation::updateAnimation(float dt)
{
	mKeyFrameDuration += dt;
	if (mKeyFrameDuration >= mAnimationSpeed)
	{
		mCurrentKeyFrame.x++;

		mCurrentKeyFrame.x %= mSpriteSheetWidth;

		setTextureRect(sf::IntRect(mCurrentKeyFrame.x * mKeyFrameSize.x,
			mCurrentKeyFrame.y * mKeyFrameSize.y, mKeyFrameSize.x, mKeyFrameSize.y));
		mKeyFrameDuration = 0.0f;
	}
}

void Animation::updateAnimationBouncing(float dt)
{
	mKeyFrameDuration += dt;
	if (mKeyFrameDuration >= mAnimationSpeed)
	{
		mCurrentKeyFrame.x += mSheetDirection;

		if (mCurrentKeyFrame.x >= mSpriteSheetWidth - 1)
			mSheetDirection *= -1;
		if (mCurrentKeyFrame.x == -1)
		{
			mSheetDirection = 1;
			setCurrentKeyFrameX(1);
		}

		setTextureRect(sf::IntRect(mCurrentKeyFrame.x * mKeyFrameSize.x,
			mCurrentKeyFrame.y * mKeyFrameSize.y, mKeyFrameSize.x, mKeyFrameSize.y));

		mKeyFrameDuration = 0.0f;
	}
}
