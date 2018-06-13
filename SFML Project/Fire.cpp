#include "Fire.h"

void Fire::extinguish()
{
	mCurrentKeyFrame.x = mSpriteSheetWidth; // Makes fire invisible
	mOnBlock = false;
	mIsBurning = false;
	mIsDeployed = false;
	mIsBlockDestroyer = false;
}

void Fire::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSpriteSheet, states);
}

Fire::Fire(Part part)
{
	// Initialise class variables
	mDuration = 1.4f;
	mPart = part;
	mIsBurning = false;
	mOnBlock = false;
	mIsDeployed = false;

	// Initialise animation variables.
	mKeyFrameSize = sf::Vector2i(32, 32);
	mCurrentKeyFrame = sf::Vector2i(0, mPart);
	mSpriteSheetWidth = 4;
	mAnimationSpeed = mDuration / 7;
	mKeyFrameDuration = 0.0f;
	mSheetDirection = 1;

	mTexture.loadFromFile("../Resources/Fire.png");
	mSpriteSheet.setTexture(mTexture);
	mSpriteSheet.setTextureRect(sf::IntRect(mKeyFrameSize.x * mSpriteSheetWidth, mKeyFrameSize.y*mPart,
		mKeyFrameSize.x, mKeyFrameSize.y));

}

Fire::~Fire()
{
}

float Fire::getDuration() const
{
	return mDuration;	
}

bool Fire::getDeployed() const
{
	return mIsDeployed;
}

bool Fire::getBurning() const
{
	return mIsBurning;
}

bool Fire::getBlockDestroyer() const
{
	return mIsBlockDestroyer;
}

bool Fire::ignite()
{
	if (mOnBlock)
	{
		mIsBlockDestroyer = true;
	}
	else
	{
		mIsBurning = true;
		mSheetDirection = 1;
		mKeyFrameDuration = 0.0f;
		mCurrentKeyFrame.x = 0;
		mSpriteSheet.setTextureRect(sf::IntRect(mCurrentKeyFrame.x * mKeyFrameSize.x,
			mCurrentKeyFrame.y * mKeyFrameSize.y, mKeyFrameSize.x, mKeyFrameSize.y));		
	}
	return mOnBlock;
}

void Fire::prepFire(sf::Vector2f position)
{
	mOnBlock = false;
	mIsDeployed = true;
	mIsBlockDestroyer = false;
	mSpriteSheet.setPosition(position);
	mCurrentKeyFrame.x = mSpriteSheetWidth;
}

void Fire::prepFire(int xPos, int yPos)
{
	prepFire(sf::Vector2f(xPos, yPos));
}

void Fire::setPart(Part part)
{
	mPart = part;
}

void Fire::encounterBlock(bool isOnBlock)
{
	mOnBlock = isOnBlock;
}

sf::FloatRect Fire::getGlobalBounds() const
{
	// Slightly scaled to not accidently collide with blocks to the side.
	sf::FloatRect tempRect = mSpriteSheet.getGlobalBounds();
	float scale = 0.9;
	tempRect.top += tempRect.height * ((1 - scale) / 2);
	tempRect.left += tempRect.width  * ((1 - scale) / 2);
	tempRect.height *= scale;
	tempRect.width *= scale;
	return tempRect;	
}

void Fire::update(float dt)
{
	if (mIsBurning)
	{
		mKeyFrameDuration += dt;
		// Update animation
		if (mKeyFrameDuration >= mAnimationSpeed)
		{
			mCurrentKeyFrame.x += mSheetDirection;

			if (mCurrentKeyFrame.x >= mSpriteSheetWidth - 1)
				mSheetDirection *= -1;

			else if (mCurrentKeyFrame.x < 0)
			{
				extinguish();
			}

			mSpriteSheet.setTextureRect(sf::IntRect(mCurrentKeyFrame.x * mKeyFrameSize.x,
				mCurrentKeyFrame.y * mKeyFrameSize.y, mKeyFrameSize.x, mKeyFrameSize.y));

			mKeyFrameDuration = 0.0f;
		}
	}
}

