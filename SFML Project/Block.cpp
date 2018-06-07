#include "Block.h"


void Block::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSpriteSheet, states);
}

Block::Block(float xPos, float yPos)
{
	// Load texture, set it to the sprite and set what part of the sprite sheet to draw.
	if (mTexture.loadFromFile("../Resources/block.png"))
	{
		// Handle error
	}
	mSpriteSheet.setTexture(mTexture);
	mSpriteSheet.setTextureRect(sf::IntRect(0, 0, 32, 32));

	mSolid = true;
	mSpriteSheet.setPosition(xPos, yPos);

}

Block::Block(std::string texDir, bool solid, float xPos, float yPos)
{
	// Load texture, set it to the sprite and set what part of the sprite sheet to draw.
	if (mTexture.loadFromFile(texDir))
	{
		// Handle error
	}
	mSpriteSheet.setTexture(mTexture);
	mSpriteSheet.setTextureRect(sf::IntRect(0, 0, 32, 32));

	mSolid = solid;
	mSpriteSheet.setPosition(xPos, yPos);
}

Block::Block(sf::Vector2f position)
	:Block(position.x, position.y)
{
}


Block::~Block()
{
}

sf::FloatRect Block::getGlobalBounds() const
{
	return mSpriteSheet.getGlobalBounds();
}

sf::Vector2f Block::getPosition() const
{
	return mSpriteSheet.getPosition();
}

sf::Sprite & Block::getSpriteRef()
{
	return mSpriteSheet;
}

sf::Sprite Block::getSprite() const
{
	return mSpriteSheet;
}

bool Block::getIsSolid() const
{
	return mSolid;
}

