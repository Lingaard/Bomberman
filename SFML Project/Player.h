#ifndef PLAYER_H
#define PLAYER_H
#include "Bomb.h"
#include "Animation.h"

class Player : public Animation
{
public: 
	enum direction { left, up, right, down};
private:
	static const int BOMBCAP = 5;
	//Class variables
	Bomb mBomb[BOMBCAP]; 
	float mSpeed;
	int mLives;
	int mNrOfBombs;
	float mSafe;
	float mSafeTime; //Time of invinsibility after getting hurt
	float mTeleportTime;
	bool mBombWasPressed;
	float mBlockInDirection[4];

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
	void kill();
	sf::Vector2f getCenteredPosition()const;
public:
	Player(int playerNumber = 1, int inputMode = 1, sf::Vector2f startPosition = sf::Vector2f(32,32));
	virtual ~Player();
	void update(float dt);	

	bool getIsFireDeployed(int bombIndex, int fireIndex)const;
	int getNrOfBombs()const;
	int getNrOfFires()const;
	int getLives()const;
	bool isAlive()const;
	bool canTeleport() const;	
	sf::FloatRect getBombGlobalBounds(int index)const; 
	
	Fire* getFire(int iBomb, int iFire);
	 
	
	void takeDamage();
	void encounterBlock(int direction);
	void encounterBlockReset();
	void hasTeleported();

	void explodeBomb(int bombIndex);
	void activatePickup(int type);
	
};
#endif