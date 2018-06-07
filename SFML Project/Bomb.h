#ifndef BOMB_H
#define BOMB_H
#include "Fire.h"
class Bomb:public sf::Drawable
{
public:
	enum direction { left, up, right, down };
private:
	// Class variables
	Fire** mFires;
	int mNrOfFires;
	int mRange;
	float mFuseTime; // Time from dropping bomb to explosion
	float mFuse;     // the timer that counts down
	float mCoolDown;
	bool mBlownUp;
	sf::Vector2f mDirection;
	// Texture variables
	sf::Texture mTexture;
	sf::Sprite mSpriteSheet;
	// Animation variables
	sf::Vector2i mCurrentKeyFrame;
	sf::Vector2i mKeyFrameSize;
	int mSpriteSheetWidth;
	float mAnimationSpeed;
	float mKeyFrameDuration;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void expand();
	void freeMemory();
public:
	Bomb();
	Bomb(int fuse);
	Bomb(const Bomb& origin);
	virtual~Bomb();
	void Update(float dt);

	bool getIsFireDeployed(int index)const;
	bool getIsFireBlockDestroyer(int index)const;
	sf::FloatRect getBombGlobalBounds()const;
	Fire* getFire(int index);
	void increaseRange();
	void explode();
	void prepExplode(sf::Vector2f position);
	bool dropBomb(sf::Vector2f position);
	int getNrOfFires()const;
	void fireOnBlock(int index, bool isOnBlock);
};



#endif // !BOMB_H