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

void Teleporter::teleport(sf::Sprite & player, sf::Vector2f telePosArr[], int arrLen, sf::Vector2f doNotTeleport)
{
	int destination;
	do  // Find a teleporter other than this one
	{
		destination = rand() % arrLen;
	} while (telePosArr[destination] == doNotTeleport);
	player.setPosition(telePosArr[destination]);
}

