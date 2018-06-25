#include "Graphics.h"

void Graphics::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::setTexture(std::string fileDir)
{
	mTexture.loadFromFile(fileDir);
	mSprite.setTexture(mTexture);
}

void Graphics::setPosition(sf::Vector2f position)
{
	mSprite.setPosition(position);
}

void Graphics::setPosition(float x, float y)
{
	mSprite.setPosition(x, y);
}

void Graphics::setTextureRect(sf::IntRect rect)
{
	mSprite.setTextureRect(rect);
}

void Graphics::setColor(const sf::Color& color)
{
	mSprite.setColor(color);
}

sf::Vector2f Graphics::getPosition() const
{
	return mSprite.getPosition();
}

sf::FloatRect Graphics::getGlobalBounds() const
{
	return mSprite.getGlobalBounds();
}

sf::FloatRect Graphics::getGlobalBoundsScaled(float scale) const
{
	sf::FloatRect tempRect = mSprite.getGlobalBounds();

	tempRect.top += tempRect.height * ((1 - scale) / 2);
	tempRect.left += tempRect.width  * ((1 - scale) / 2);
	tempRect.height *= scale;
	tempRect.width *= scale;
	return tempRect;
}

const sf::Sprite & Graphics::getSprite() const
{
	return mSprite;
}

void Graphics::moveSprite(const sf::Vector2f & offset)
{
	mSprite.move(offset);
}

void Graphics::moveSprite(float offsetX, float offsetY)
{
	mSprite.move(offsetX, offsetY);
}
