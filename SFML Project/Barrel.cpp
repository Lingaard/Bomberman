#include "Barrel.h"



void Barrel::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(getSprite(), states);
	target.draw(mBomb, states);
}

Barrel::Barrel(float xPos, float yPos)
	:Block("../Resources/Barrel.png", true, xPos, yPos)
{
	mBomb.setFuseTime(0.1f);
	mBomb.increaseRange();
	mBomb.increaseRange();
}


Barrel::~Barrel()
{
}

void Barrel::update(float dt)
{
	mBomb.update(dt);
}

void Barrel::explode()
{
	mBomb.dropBomb(getPosition());
	getSpriteRef().setPosition(-32, -32);
}



int Barrel::getNrOfFires() const
{
	return mBomb.getNrOfFires();
}

Fire * Barrel::getFire(int iFire)
{
	return mBomb.getFire(iFire);
}

