#include "Pickup.h"

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

	setTexture("../Resources/Pickup.png");
	setTextureRect(sf::IntRect(32 * mType, 0, 32, 32));

	setPosition(position);
}

Pickup::Pickup(float xPos, float yPos)
	:Pickup(sf::Vector2f(xPos,yPos))
{
}

Pickup::~Pickup()
{
}

int Pickup::getType()
{
	return mType;
}

