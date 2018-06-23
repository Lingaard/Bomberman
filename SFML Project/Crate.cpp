#include "Crate.h"

Crate::Crate(float xPos, float yPos)
	:Block("../Resources/Crate.png", true, xPos, yPos)
{
	int chance = rand() % 100 + 1;
	mHasItem = chance <= 20;
	//mHasItem = true;
}

Crate::~Crate()
{
}

bool Crate::getHasItem() const
{
	return mHasItem;
}

void Crate::destroy()
{
	setPosition(-32, -32);
}




