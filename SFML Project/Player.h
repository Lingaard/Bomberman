#ifndef PLAYER_H
#define PLAYER_H
#include "Bomb.h"
class Player : public sf::Drawable
{
public: 
	enum direction { left, up, right, down};
private:
	static const int BOMBCAP = 5;
	//Class variables
	Bomb mBomb[BOMBCAP]; 
	float mSpeed;
	int mLives;
	//bool mCanKick;
	int mNrOfBombs;
	float mSafe;
	float mSafeTime; //Time of invinsibility after getting hurt
	float mTeleportTime;
	bool mBombWasPressed;
	float mBlockInDirection[4];

	// Texture variables
	sf::Texture mTexture;
	sf::Sprite mSpriteSheet;
	// Animation variables
	sf::Vector2i mCurrentKeyFrame;
	sf::Vector2i mKeyFrameSize;
	int mSpriteSheetWidth;
	float mAnimationSpeed;
	float mKeyFrameDuration;


	// Input variables
	sf::Keyboard::Key mInLeft;
	sf::Keyboard::Key mInUp;
	sf::Keyboard::Key mInRight;
	sf::Keyboard::Key mInDown;
	sf::Keyboard::Key mInBomb;


	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void setControlStd(int inputMode);
	void dropBomb();
	void increaseRange();
	sf::Vector2f getCenteredPosition()const;
public:
	Player(int playerNumber = 1, int inputMode = 1, sf::Vector2f startPosition = sf::Vector2f(32,32));
	virtual ~Player();
	void Update(float dt);	

	/*bool getIsFireBurning(int bombIndex, int fireIndex)const;*/
	bool getIsFireDeployed(int bombIndex, int fireIndex)const;
	//bool getIsFireBlockDestroyer(int bombIndex, int fireIndex)const;
	int getNrOfBombs()const;
	int getNrOfFires()const;
	sf::FloatRect getPlayerGlobalBounds(float scale = 0.9f)const;
	sf::FloatRect getBombGlobalBounds(int index)const;
	//sf::FloatRect getFireGlobalBounds(int bombIndex, int fireIndex)const;
	sf::Sprite& getSprite();
	Fire* getFire(int iBomb, int iFire);
	 
	void takeDamage();
	void kill();
	void encounterBlock(int direction);
	void encounterBlockReset();
	bool canTeleport();
	void hasTeleported();
	bool isAlive()const;

	void explodeBomb(int bombIndex);
	void fireOnBlock(int bombIndex, int fireIndex, bool isOnBlock);
	void activatePickup(int type);
	
};
#endif