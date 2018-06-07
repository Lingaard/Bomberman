#ifndef GAME_HPP
#define GAME_HPP

#include <SFML\Graphics.hpp>
#include "Player.hpp"

class Game : public sf::Drawable
{
public:
	Game();

	void Update(float dt);

private:
	sf::Texture mBackgroundTex;
	sf::Sprite mBackgroundSprite;
	Player mPlayer;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
#endif