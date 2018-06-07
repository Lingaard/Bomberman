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

void Teleporter::teleport(sf::Sprite & target, sf::Vector2f telePosArr[], int arrLen, sf::Vector2f doNotTele)
{
	int destination;
	do
	{
		destination = rand() % arrLen;
	} while (telePosArr[destination] == doNotTele);
	target.setPosition(telePosArr[destination]);
}

