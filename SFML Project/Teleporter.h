#ifndef TELEPORTER_H
#define TELEPORTER_H
#include "Block.h"

class Teleporter:public Block
{
private:

public:
	Teleporter(float xPos = 0.0f, float yPos = 0.0f);
	~Teleporter();

	sf::Vector2f getPosition()const;
	sf::Vector2f teleport(sf::Vector2f telePosArr[], int arrLen);
};

#endif // !TELEPORTER_H
