#ifndef ANIMATION_H
#define ANIMATION_H
#include "Graphics.h"

class Animation: public Graphics
{
private:

	sf::Vector2i mCurrentKeyFrame;
	sf::Vector2i mKeyFrameSize;
	int mSpriteSheetWidth;
	float mAnimationSpeed;
	float mKeyFrameDuration;

	int mSheetDirection; // Used in bouncing

public:
	Animation();
	virtual ~Animation();

	void setSpriteSheetWidth(int width);
	void setAnimationSpeed(float speed);
	void setKeyFrameDuration(float duration);
	void setCurrentKeyFrameY(int y);
	void setCurrentKeyFrameX(int x);
	void increaseCurrentKeyFrameX();
	

	void updateAnimation(float dt);
	void updateAnimationBouncing(float dt);
};

#endif //ANIMATION_H
