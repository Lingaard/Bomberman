#ifndef BARREL_H
#define BARREL_H
#include "Block.h"
#include "Bomb.h"
class Barrel :public Block
{
private:
	Bomb mBomb;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
	Barrel(float xPos, float yPos);
	virtual~Barrel();

	void update(float dt);
	void explode();

	int getNrOfFires()const;
	Fire* getFire(int iFire);
};

#endif // !BARREL_H
