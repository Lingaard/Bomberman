#include "Block.h"

Block::Block(float xPos, float yPos)
{	
	setTexture("../Resources/block.png");
	setTextureRect(sf::IntRect(0, 0, 32, 32));
	setPosition(xPos, yPos);

	mSolid = true;
}

Block::Block(std::string texDir, bool solid, float xPos, float yPos)
{	
	setTexture(texDir);
	setTextureRect(sf::IntRect(0, 0, 32, 32));

	mSolid = solid;
	setPosition(xPos, yPos);
}

Block::Block(sf::Vector2f position)
	:Block(position.x, position.y)
{
}


Block::~Block()
{
}

bool Block::getIsSolid() const
{
	return mSolid;
}

