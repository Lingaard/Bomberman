#ifndef COLLISIONHANDELER_H
#define COLLISIONHANDELER_H
#include "Level.h"

class CollisionHandeler
{
private:
	void centerRect(sf::FloatRect& rect);

	int  collidePlayer(Player* player, sf::FloatRect other);
	void collideFire(Fire* fire, Level& level);
	void checkCollisionPlayerAndBlocks(int iPlayer, Level& level);
	void checkCollisionPlayerAndPickups(int iPlayer, Level& level);
	void checkCollisionPlayerAndBombs(int iPlayer, Level& level);
	void checkCollisionFireFromPlayer(int iPlayer, Level& level);
	void checkCollisionFireFromBarrels(Level& level);

public:
	CollisionHandeler();
	virtual~CollisionHandeler();

	void checkAllCollisions(Level& level);
};
#endif // !COLLISIONHANDELER_H
