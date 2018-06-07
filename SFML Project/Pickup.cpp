#include "Pickup.h"




void Pickup::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSpriteSheet, states);
}

void Pickup::pickType()
{
	int random = rand() % 90 + 1;
	
	if (random < 30)
	{
		mType = range;
	}
	else if (random < 60)
	{
		mType = amount;
	}
	else 
	{
		mType = speed;
	}
}

Pickup::Pickup(sf::Vector2f position)
{
	pickType();

	// Load texture, set it to the sprite and set what part of the sprite sheet to draw.
	if (mTexture.loadFromFile("../Resources/Pickup.png"))
	{
		// Handle error
	}
	mSpriteSheet.setTexture(mTexture);
	mSpriteSheet.setTextureRect(sf::IntRect(32 * mType, 0, 32, 32));

	mSpriteSheet.setPosition(position);
}

Pickup::Pickup(float xPos, float yPos)
	:Pickup(sf::Vector2f(xPos,yPos))
{
}

Pickup::~Pickup()
{
}




sf::FloatRect Pickup::getGlobalBounds() const
{
	return mSpriteSheet.getGlobalBounds();
}

int Pickup::getType()
{
	return mType;
}

