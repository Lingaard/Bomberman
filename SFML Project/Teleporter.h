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
	void teleport(sf::Sprite& target, sf::Vector2f telePosArr[], int arrLen, sf::Vector2f doNotTele);
};

#endif // !TELEPORTER_H
