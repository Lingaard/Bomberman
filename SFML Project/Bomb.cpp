#include "Bomb.h"


void Bomb::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSpriteSheet, states);
	for (int i = 0; i < mNrOfFires; i++)
	{
		target.draw(*mFires[i], states);				
	}
}

void Bomb::expand()
{
	Fire** temp = new Fire*[mNrOfFires];
	for (int i = 0; i < mNrOfFires-4; i++)
	{
		temp[i] = mFires[i];
	}
	for (int i = mNrOfFires - 4; i < mNrOfFires; i++)
	{
		temp[i] = nullptr;
	}
	delete[]mFires;
	mFires = temp;
}

void Bomb::freeMemory()
{
	for (int i = 0; i < mNrOfFires; i++)
	{
		delete mFires[i];
	}
	delete[]mFires;
}



void Bomb::increaseRange()
{
	mNrOfFires += 4;
	expand();
	mRange++;
	for (int i = mNrOfFires - 4; i < mNrOfFires; i++)
	{
		if (i % 2 == 1)
			mFires[i] = new Fire(Fire::horisontal);
		else
			mFires[i] = new Fire(Fire::vertical);
	}
}



Bomb::Bomb()
{
	// Initialise class variables.
	mCoolDown = 0.0f;
	mFuseTime = 1.5f;
	mFuse = 0.0f;
	mNrOfFires = 5;
	mRange = 1;
	mBlownUp = false;
	mFires = new Fire*[mNrOfFires];		//Sets odd as horisontal and even as vertical.
	mFires[0] = new Fire(Fire::center);
	mFires[1] = new Fire(Fire::left);
	mFires[2] = new Fire(Fire::top);
	mFires[3] = new Fire(Fire::right);
	mFires[4] = new Fire(Fire::bottom);

	mSpriteSheet.setPosition(-32, -32);

		// Load texture, set it to the sprite and set what part of the sprite sheet to draw.
	if (mTexture.loadFromFile("../Resources/Bomb.png"))
	{
		// Handle error
	}
	mSpriteSheet.setTexture(mTexture);
	mSpriteSheet.setTextureRect(sf::IntRect(0, 0, 32, 32));
	
	// Initialise animation variables.
	mCurrentKeyFrame = sf::Vector2i(0, 0);
	mKeyFrameSize = sf::Vector2i(32, 32);
	mSpriteSheetWidth = 4; 
	mAnimationSpeed = mFuseTime / 7;
	mKeyFrameDuration = 0.0f;
}

Bomb::Bomb(int fuse)
	:Bomb() 
{
	mFuseTime = fuse;
}

Bomb::Bomb(const Bomb & origin)
{
	// Member wise copying.
	this->mCoolDown  = origin.mCoolDown;
	this->mFuseTime  = origin.mFuseTime;
	this->mFuse		 = origin.mFuse;
	this->mNrOfFires = origin.mNrOfFires;
	this->mRange	 = origin.mRange;
	this->mBlownUp	 = origin.mBlownUp;
	this->mFires	 = new Fire*[mNrOfFires];		

	for (int i = 0; i < mNrOfFires; i++) // Deep copying
	{
		mFires[i] = new Fire(*origin.mFires[i]);
	}

	// Load texture, set it to the sprite and set what part of the sprite sheet to draw.
	if (mTexture.loadFromFile("../Resources/Bomb.png"))
	{
		// Handle error
	}
	mSpriteSheet.setTexture(mTexture);
	mSpriteSheet.setTextureRect(origin.mSpriteSheet.getTextureRect());

	// Copy animation variables.
	this->mCurrentKeyFrame	= origin.mCurrentKeyFrame;
	this->mKeyFrameSize		= origin.mKeyFrameSize;
	this->mSpriteSheetWidth = origin.mSpriteSheetWidth;
	this->mAnimationSpeed	= origin.mAnimationSpeed;
	this->mKeyFrameDuration = origin.mKeyFrameDuration;
}


Bomb::~Bomb()
{
	freeMemory();
}

void Bomb::Update(float dt)
{
	if (mCoolDown > 0.0f) 
	{
		mCoolDown -= dt;
		for (int i = 0; i < mNrOfFires; i++)
		{
			mFires[i]->Update(dt);
		}

		if (mFuse > 0.0f)
		{
			mFuse -= dt;

			// Update animation for bomb
			mKeyFrameDuration += dt;
			if (mKeyFrameDuration >= mAnimationSpeed)
			{
				mCurrentKeyFrame.x++;

				mCurrentKeyFrame.x %= mSpriteSheetWidth;

				mSpriteSheet.setTextureRect(sf::IntRect(mCurrentKeyFrame.x * mKeyFrameSize.x,
					mCurrentKeyFrame.y * mKeyFrameSize.y, mKeyFrameSize.x, mKeyFrameSize.y));
				mKeyFrameDuration = 0.0f;
			}
		}
		else if (!mBlownUp)
		{
			explode();
		}
	}
	else
	{
		mBlownUp = false;
	}
}

void Bomb::explode()
{
	mBlownUp = true;
	mSpriteSheet.setPosition(-32, -32);

	enum direction { left, up, right, down };
	mFires[0]->setFire();
	bool collideWithBlock[4] = { false };

	// Ignites the horisontal/vertical Fires
	for (int i = 1; i < mRange && !collideWithBlock[left]; i++)
	{
		collideWithBlock[left] = mFires[1 + left + 4 * i]->setFire();
	}
	for (int i = 1; i < mRange && !collideWithBlock[up]; i++)
	{
		collideWithBlock[up] = mFires[1 + up + 4 * i]->setFire();
	}
	for (int i = 1; i < mRange && !collideWithBlock[right]; i++)
	{
		collideWithBlock[right] = mFires[1 + right + 4 * i]->setFire();
	}
	for (int i = 1; i < mRange && !collideWithBlock[down]; i++)
	{
		collideWithBlock[down] = mFires[1 + down + 4 * i]->setFire();
	}

	// Ignites the ends
	if (!collideWithBlock[left])
	{
		mFires[1 + left]->setFire();
	}
	if (!collideWithBlock[up])
	{
		mFires[1 + up]->setFire();
	}
	if (!collideWithBlock[right])
	{
		mFires[1 + right]->setFire();
	}
	if (!collideWithBlock[down])
	{
		mFires[1 + down]->setFire();
	}
}

void Bomb::prepExplode(sf::Vector2f position)
{
	mFires[0]->prepFire(position);

	for (int i = 1; i < mRange; i++)
	{
		mFires[1 + left + 4 * i]->prepFire(position.x - i * 32, position.y);
		mFires[1 + up + 4 * i]->prepFire(position.x, position.y - i * 32);
		mFires[1 + right + 4 * i]->prepFire(position.x + i * 32, position.y);
		mFires[1 + down + 4 * i]->prepFire(position.x, position.y + i * 32);
	}

	mFires[1 + left]->prepFire(position.x - mRange * 32, position.y);
	mFires[1 + up]->prepFire(position.x, position.y - mRange * 32);
	mFires[1 + right]->prepFire(position.x + mRange * 32, position.y);
	mFires[1 + down]->prepFire(position.x, position.y + mRange * 32);
}

bool Bomb::dropBomb(sf::Vector2f position)
{//Returns true if bomb is available to be dropped. false if it isn't
	bool dropSuccessful = false;
	if (mCoolDown <= 0.0f)
	{
		dropSuccessful = true;
		mSpriteSheet.setPosition(position);
		mFuse = mFuseTime;
		mCoolDown = mFuseTime + mFires[0]->getDuration() + 0.2f;
		prepExplode(position);
	}

	return dropSuccessful;
}

int Bomb::getNrOfFires() const
{
	return mNrOfFires;
}

void Bomb::fireOnBlock(int index, bool isOnBlock)
{
	mFires[index]->encounterBlock(isOnBlock);
}

bool Bomb::getIsFireDeployed(int index) const
{
	return mFires[index]->getDeployed();
}

bool Bomb::getIsFireBlockDestroyer(int index) const
{
	return mFires[index]->getBlockDestroyer();
}

sf::FloatRect Bomb::getBombGlobalBounds() const
{
	return mSpriteSheet.getGlobalBounds();
}

Fire * Bomb::getFire(int index)
{
	return mFires[index];
}