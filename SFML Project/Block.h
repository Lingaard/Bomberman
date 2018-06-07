#ifndef BLOCK_H
#define BLOCK_H
#include <SFML\Graphics.hpp>

class Block :public sf::Drawable
{
private:
	// Class variables
	bool mSolid;


	// Texture variables
	sf::Texture mTexture;
	sf::Sprite mSpriteSheet;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
	Block(float xPos = 0.0f, float yPos = 0.0f);
	Block(std::string texDir, bool solid, float xPos = 0.0f, float yPos = 0.0f);
	Block(sf::Vector2f position);
	virtual~Block();

	sf::FloatRect getGlobalBounds()const;
	sf::Vector2f getPosition()const;
	sf::Sprite& getSpriteRef();
	sf::Sprite getSprite()const;
	bool getIsSolid()const;
};


#endif // !BLOCK_H
