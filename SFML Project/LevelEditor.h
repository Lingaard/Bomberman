#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H
#include <SFML\Graphics.hpp>

class LevelEditor:public sf::Drawable
{
private:
	// Texture variables
	sf::Texture mTexture;
	sf::Sprite mSpriteSheet;

	enum BlockType{empty, block, crate, teleporter, barrel};
	BlockType mTypeMode;
	BlockType mBlockTypes[19 * 19] = {empty}; // (361)
	sf::Sprite mBlocks[19 * 19];
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
	LevelEditor();
	~LevelEditor();
};

#endif // !LEVELEDITOR_H
