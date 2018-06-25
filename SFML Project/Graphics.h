#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SFML\Graphics.hpp>

class Graphics :public sf::Drawable
{
private:
	sf::Texture mTexture;
	sf::Sprite mSprite;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
	Graphics();
	virtual~Graphics();

	void setTexture(std::string fileDir);
	void setPosition(sf::Vector2f position);
	void setPosition(float x, float y);
	void setTextureRect(sf::IntRect rect);
	void setColor(const sf::Color& color);
	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds() const;
	sf::FloatRect getGlobalBoundsScaled(float scale = 0.9f) const;
	const sf::Sprite& getSprite() const;

	void moveSprite(const sf::Vector2f& offset);
	void moveSprite(float offsetX, float offsetY);
};
#endif // !GRAPHICS_H
