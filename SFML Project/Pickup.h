#ifndef PICKUP_H
#define PICKUP_H
#include "Graphics.h"

class Pickup:public Graphics
{
public:
	enum PickupName { range, amount, speed, kick};
private:
	int mType;



	void pickType();
public:
	Pickup(sf::Vector2f position);
	Pickup(float xPos, float yPos);
	virtual~Pickup();
	
	int getType();
};

#endif // !PICKUP_H
