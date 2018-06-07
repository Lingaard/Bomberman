#ifndef PICKUP_H
#define PICKUP_H
#include<SFML\Graphics.hpp>

class Pickup:public sf::Drawable
{
public:
	enum PickupName { range, amount, speed, kick};
private:
	int mType;
	bool mDeployed;

	// Texture variables
	sf::Texture mTexture;
	sf::Sprite mSpriteSheet;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void pickType();
public:
	Pickup(sf::Vector2f position);
	Pickup(float xPos, float yPos);
	virtual~Pickup();
	
	sf::FloatRect getGlobalBounds()const;
	int getType();
};

#endif // !PICKUP_H
