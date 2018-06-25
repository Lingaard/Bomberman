#ifndef FIRE_H
#define FIRE_H
#include "Animation.h"

class Fire:public Animation
{
public:
	const enum Part { bottom, top, right, left, vertical, horisontal, center };
private:
	// Class variables
	float mDuration;  // Duration of burning
	float mTimeLit;
	Part mPart; 
	bool mOnBlock;    // True if fire is set on a block. Used to stop fire spread
	bool mIsBurning;  
	bool mIsDeployed;
	bool mIsBlockDestroyer; // True if first fire on a block in a direction

	void extinguish();
public:
	Fire(Part part = top);
	virtual~Fire();
	void update(float dt);

	float getDuration()const;
	bool getDeployed()const;
	bool getBurning()const;
	bool getBlockDestroyer()const;
	bool ignite();
	void prepFire(sf::Vector2f position);
	void prepFire(float xPos, float yPos); 
	void setPart(Part part);
	void encounterBlock(bool isOnBlock);
};


#endif // !FIRE_H
