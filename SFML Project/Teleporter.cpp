#include "Teleporter.h"


Teleporter::Teleporter(float xPos, float yPos)
	:Block("../Resources/Teleporter.png", false, xPos, yPos)
{
	
}

Teleporter::~Teleporter()
{
}

sf::Vector2f Teleporter::getPosition() const
{
	return sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top);
}

sf::Vector2f Teleporter::teleport(sf::Vector2f telePosArr[], int arrLen)
{
	int destination;
	if (arrLen != 1)
	{
		do  // Find a teleporter other than this one
		{
			destination = rand() % arrLen;
		} while (telePosArr[destination] == getPosition());
	}
	else
		destination = 0;
	return telePosArr[destination];
}

