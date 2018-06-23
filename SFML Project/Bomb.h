#ifndef BOMB_H
#define BOMB_H
#include "Fire.h"
#include "Animation.h"

class Bomb:public Animation
{
public:
	enum direction { left, up, right, down };
private:
	// Class variables
	Fire** mFires;
	int mNrOfFires;
	int mRange;
	float mFuseTime; // Time from dropping bomb to explosion
	float mFuse;     // The timer that counts down
	float mCoolDown;
	bool mBlownUp;
	sf::Vector2f mDirection;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void expand();
	void freeMemory();
public:
	Bomb();
	virtual~Bomb();
	void update(float dt);

	void setFuseTime(float time);
	bool getIsFireDeployed(int index)const;
	bool getIsFireBlockDestroyer(int index)const;
	Fire* getFire(int index);
	int getNrOfFires()const;

	void increaseRange();
	void explode();
	void prepExplode(sf::Vector2f position);
	bool dropBomb(sf::Vector2f position);
};



#endif // !BOMB_H
