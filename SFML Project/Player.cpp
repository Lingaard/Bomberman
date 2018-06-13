#include "Player.h"

Player::Player(int playerNumber, int inputMode, sf::Vector2f startPosition)
{	
	sf::String fileName = "../Resources/player"+std::to_string(playerNumber)+".png";

	mTexture.loadFromFile(fileName);
	
	mSpriteSheet.setTexture(mTexture);
	mSpriteSheet.setTextureRect(sf::IntRect(0, 0, 32, 32));
													  
	// Initialise animation variables.				  
	mCurrentKeyFrame = sf::Vector2i(0, 0);			  
	mKeyFrameSize = sf::Vector2i(32, 32);			  
	mSpriteSheetWidth = 4;
	mAnimationSpeed = 0.2f;
	mKeyFrameDuration = 0.0f;

	//Initiate class variables
	mNrOfBombs = 1;
	mLives = 3;
	mSpeed = 180.0f;
	mSafe = 0.0f;
	mSafeTime = 3.0f;
	mTeleportTime = 0.0f;
	mBombWasPressed = false;
	setControlStd(inputMode);
	
	increaseRange();
	encounterBlockReset();
	
	mSpriteSheet.setPosition(startPosition);
}

Player::~Player()
{
}

void Player::update(float dt)
{
	if (mLives > 0) // if alive
	{
		sf::Vector2f direction(0.0f, 0.0f);

		// Handle movement input and update direction and animation
		if (sf::Keyboard::isKeyPressed(mInLeft))
		{
			mKeyFrameDuration += dt;
			mCurrentKeyFrame.y = 1;
			direction.x += -1.0f * mBlockInDirection[left];	
			//mBlockInDirection reverses the movement if colliding with block.
		}
		if (sf::Keyboard::isKeyPressed(mInRight))
		{
			mKeyFrameDuration += dt;
			mCurrentKeyFrame.y = 2;
			direction.x += 1.0f * mBlockInDirection[right];
		}
		if (sf::Keyboard::isKeyPressed(mInDown))
		{
			mKeyFrameDuration += dt;
			mCurrentKeyFrame.y = 0;
			direction.y += 1.0f * mBlockInDirection[down];
		}
		if (sf::Keyboard::isKeyPressed(mInUp))
		{
			mKeyFrameDuration += dt;
			mCurrentKeyFrame.y = 3;
			direction.y += -1.0f * mBlockInDirection[up];
		}
		if (sf::Keyboard::isKeyPressed(mInBomb) && !mBombWasPressed)
		{
			dropBomb();
			mBombWasPressed = true;
		}

		// Gives a constant speed
		if (abs(direction.x) + abs(direction.y) == 2)
		{
			direction *= 0.71f;
		}
		
		mSpriteSheet.move(direction * mSpeed * dt);

		// Update animation
		if (mKeyFrameDuration >= mAnimationSpeed)
		{
			mCurrentKeyFrame.x++;

			mCurrentKeyFrame.x %= mSpriteSheetWidth;

			mSpriteSheet.setTextureRect(sf::IntRect(mCurrentKeyFrame.x * mKeyFrameSize.x,
				mCurrentKeyFrame.y * mKeyFrameSize.y, mKeyFrameSize.x, mKeyFrameSize.y));
			mKeyFrameDuration = 0.0f;
		}

		// Update safe mode
		if (mSafe > 0)
		{
			mSafe -= dt;
			mSpriteSheet.setColor(sf::Color::Red);
		}
		else
		{
			mSpriteSheet.setColor(sf::Color::White);
		}
		// Update teleport time
		if (mTeleportTime > 0)
		{
			mTeleportTime -= dt;
		}

		// Check if bomb button is released
		if (!sf::Keyboard::isKeyPressed(mInBomb))
		{
			mBombWasPressed = false;
		}

	}
	// Update all bombs
	for (int i = 0; i < mNrOfBombs; i++)
	{
		mBomb[i].update(dt);
	}

}

bool Player::getIsFireDeployed(int bombIndex, int fireIndex) const
{
	return mBomb[bombIndex].getIsFireDeployed(fireIndex);
}

void Player::takeDamage()
{
	if (mSafe <= 0)
	{
		mSafe = mSafeTime;
		mLives--;
	}
	if (mLives == 0)
	{
		kill();
	}
}

void Player::kill()
{
	mSpriteSheet.setPosition(-32, -32);
}

void Player::encounterBlock(int direction)
{
	mBlockInDirection[direction] = -1.0f;
}

void Player::encounterBlockReset()
{
	for (int i = 0; i < 4; i++)
	{
		mBlockInDirection[i] = 1.0f;
	}
}

bool Player::canTeleport() const
{
	return mTeleportTime <= 0;
}

void Player::hasTeleported()
{
	mTeleportTime = 1.0f;	
}

bool Player::isAlive() const
{
	return mLives > 0;
}

void Player::increaseRange()
{
	for (int i = 0; i < BOMBCAP; i++)
	{
		mBomb[i].increaseRange();
	}
}

void Player::explodeBomb(int bombIndex)
{
	mBomb[bombIndex].explode();
}

void Player::activatePickup(int type)
{
	enum PickupName { range, amount, speed, kick };
	switch (type)
	{
	case range:
		increaseRange();
		break;
	case amount:
		if(mNrOfBombs<5)
			mNrOfBombs++;
		break;
	case speed:
		if (mSpeed < 180 * pow(1.2, 5))
			mSpeed *= 1.2f;		
		break;
	}	
}


int Player::getNrOfBombs() const
{
	return mNrOfBombs;
}

int Player::getNrOfFires() const
{
	return mBomb[0].getNrOfFires();
}

sf::FloatRect Player::getPlayerGlobalBounds(float scale) const
{
	sf::FloatRect tempRect = mSpriteSheet.getGlobalBounds();
	
	tempRect.top  += tempRect.height * ((1 - scale) / 2);
	tempRect.left += tempRect.width  * ((1 - scale) / 2);
	tempRect.height *= scale;
	tempRect.width  *= scale;
	return tempRect;
	
}

sf::FloatRect Player::getBombGlobalBounds(int index) const
{
	return mBomb[index].getBombGlobalBounds();
}

sf::Sprite& Player::getSprite() 
{
	return mSpriteSheet;
}

Fire * Player::getFire(int iBomb, int iFire)
{
	return mBomb[iBomb].getFire(iFire);
}

int Player::getLives() const
{
	return mLives;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mSpriteSheet, states);
	for (int i = 0; i < mNrOfBombs; i++)
	{
		target.draw(mBomb[i], states);
	}
}

void Player::setControlStd(int inputMode)
{
	switch (inputMode)
	{
	case 1:		
		mInLeft  = sf::Keyboard::A;
		mInUp    = sf::Keyboard::W;
		mInRight = sf::Keyboard::D;
		mInDown  = sf::Keyboard::S;
		mInBomb  = sf::Keyboard::Space;
		break;

	case 2:		
		mInLeft  = sf::Keyboard::Left;
		mInUp    = sf::Keyboard::Up;
		mInRight = sf::Keyboard::Right;
		mInDown  = sf::Keyboard::Down;
		mInBomb  = sf::Keyboard::Return;
		break;

	case 3:
		mInLeft  = sf::Keyboard::J;
		mInUp    = sf::Keyboard::I;
		mInRight = sf::Keyboard::L;
		mInDown  = sf::Keyboard::K;
		mInBomb  = sf::Keyboard::O;
		break;

	case 4:
		mInLeft  = sf::Keyboard::Numpad4;
		mInUp    = sf::Keyboard::Numpad8;
		mInRight = sf::Keyboard::Numpad6;
		mInDown  = sf::Keyboard::Numpad5;
		mInBomb  = sf::Keyboard::Numpad9;
		break;
	}
}

void Player::dropBomb()
{	
	bool dropSuccessful = false;
	for (int i = 0; i < mNrOfBombs && !dropSuccessful; i++)
	{
 		dropSuccessful = mBomb[i].dropBomb(getCenteredPosition());
	}
}

// Returns position of the closest 32 x 32 tile under the player
// Used in bomb placement
sf::Vector2f Player::getCenteredPosition() const
{
	sf::Vector2f centeredPosition(0.0f,0.0f);
	float fmodPosX = std::fmod(mSpriteSheet.getPosition().x, 32.0f);
	float fmodPosY = std::fmod(mSpriteSheet.getPosition().y, 32.0f);

	if (fmodPosX <= 16.0f)
	{
		centeredPosition.x = mSpriteSheet.getPosition().x - fmodPosX;
	}
	else
	{
		centeredPosition.x = mSpriteSheet.getPosition().x + 32.0f -fmodPosX;
	}

	if (fmodPosY <= 16.0f)
	{
		centeredPosition.y = mSpriteSheet.getPosition().y - fmodPosY;
	}
	else
	{
		centeredPosition.y = mSpriteSheet.getPosition().y + 32.0f -fmodPosY;
	}
	return centeredPosition;
}
