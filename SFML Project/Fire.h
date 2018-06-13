#ifndef FIRE_H
#define FIRE_H
#include <SFML\Graphics.hpp>

class Fire:public sf::Drawable
{
public:
	const enum Part { bottom, top, right, left, vertical, horisontal, center };
private:
	// Class variables
	float mDuration;  // Duration of burning
	Part mPart; 
	bool mOnBlock;    // True if fire is set on a block. Used to stop fire spread
	bool mIsBurning;  
	bool mIsDeployed;
	bool mIsBlockDestroyer; // True if first fire on a block in a direction
	// Texture variables
	sf::Texture mTexture;
	sf::Sprite mSpriteSheet;
	// Animation variables
	sf::Vector2i mCurrentKeyFrame;
	sf::Vector2i mKeyFrameSize;
	int mSpriteSheetWidth;
	float mAnimationSpeed;
	float mKeyFrameDuration;
	int mSheetDirection;

	void extinguish();
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
	Fire(Part part = top);
	virtual~Fire();
	void update(float dt);

	float getDuration()const;
	bool getDeployed()const;
	bool getBurning()const;
	bool getBlockDestroyer()const;
	sf::FloatRect getGlobalBounds()const;
	bool ignite();
	void prepFire(sf::Vector2f position);
	void prepFire(int xPos, int yPos); 
	void setPart(Part part);
	void encounterBlock(bool isOnBlock);
};


#endif // !FIRE_H
