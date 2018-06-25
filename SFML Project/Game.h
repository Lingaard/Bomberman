#ifndef GAME_H
#define GAME_H
#include <SFML\Graphics.hpp>
#include "Level.h"
#include "CollisionHandeler.h"
class Game : public sf::Drawable
{	
private:
	Level mLevel;
	CollisionHandeler mCollision;
	
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
		
public:
	Game();
	virtual~Game();
	void update(float dt);

	void reset();
	int getWinner()const; // -1 still playing, player ID if winner
	void startGame(std::string levelDir, int nrOfPlayers);	
};
#endif