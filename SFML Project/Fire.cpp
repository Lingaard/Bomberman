#include "Fire.h"

void Fire::extinguish()
{
	setColor(sf::Color(0,0,0,0)); 
	mOnBlock = false;
	mIsBurning = false;
	mIsDeployed = false;
	mIsBlockDestroyer = false;
}

Fire::Fire(Part part)
{
	// Initialise class variables
	mDuration = 1.4f;
	mTimeLit = mDuration;
	mPart = part;
	mIsBurning = false;
	mOnBlock = false;
	mIsDeployed = false;

	// Initialise animation variables.
	setCurrentKeyFrameY(mPart);
	setSpriteSheetWidth(4);
	setAnimationSpeed(mDuration / 7);
	
	setTexture("../Resources/Fire.png");
	setTextureRect(sf::IntRect(0, 32*mPart,	32, 32));
	setColor(sf::Color(0, 0, 0, 0)); // Makes fire invisible
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
		mTimeLit = 0.0f;
		setColor(sf::Color::White);
		mIsBurning = true;
		setKeyFrameDuration(0.0f);
		setCurrentKeyFrameX(0);
		/*mSpriteSheet.setTextureRect(sf::IntRect(mCurrentKeyFrame.x * mKeyFrameSize.x,
			mCurrentKeyFrame.y * mKeyFrameSize.y, mKeyFrameSize.x, mKeyFrameSize.y));	*/	
	}
	return mOnBlock;
}

void Fire::prepFire(sf::Vector2f position)
{
	mOnBlock = false;
	mIsDeployed = true;
	mIsBlockDestroyer = false;
	setPosition(position);
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

void Fire::update(float dt)
{
	if (mIsBurning)
	{
		mTimeLit += dt;
		updateAnimationBouncing(dt);
		if (mTimeLit > mDuration)
			extinguish();
	}
}

