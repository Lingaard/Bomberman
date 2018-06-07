#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML\Graphics.hpp>

class Player : public sf::Drawable
{
public:
	Player(int playerNumber);

	void Update(float dt);

private:
	sf::Texture mTexture;
	sf::Sprite mSpriteSheet;
	float mSpeed = 180.0f;

	// Animation variables
	sf::Vector2i mCurrentKeyFrame;
	sf::Vector2i mKeyFrameSize;
	int mSpriteSheetWidth;
	float mAnimationSpeed;
	float mKeyFrameDuration;

	// Jump variables
	bool mWasSpacePressed = false;
	bool mIsJumping = false;
	float mStartY = 0.0f;
	float mJumpSpeed = 0.0f;
	float mGravity = 9.82f;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
#endif