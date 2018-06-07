#include "LevelEditor.h"



void LevelEditor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSpriteSheet, states);
	for (int i = 0; i < 19 * 19; i++)
	{
		target.draw(mBlocks[i], states);
	}
}

LevelEditor::LevelEditor()
{
	// Load texture, set it to the sprite and set what part of the sprite sheet to draw.
	if (mTexture.loadFromFile("../Resources/Bomb.png"))
	{
		// Handle error
	}
	mSpriteSheet.setTexture(mTexture);
	mSpriteSheet.setTextureRect(sf::IntRect(0, 0, 32, 32));

	for (int i = 0; i < 19 * 19; i++)
	{
		
	}

}


LevelEditor::~LevelEditor()
{
}
