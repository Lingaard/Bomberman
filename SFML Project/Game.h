#ifndef GAME_H
#define GAME_H
#include "Graphics.h"
#include <fstream>
#include <string>
#include <iostream>
#include "Player.h"
#include "Pickup.h"
#include "Teleporter.h"
#include "Crate.h"
#include "Barrel.h"
class Game : public Graphics
{	
private:
	static const int PLAYERCAP = 4;
	static const int SPRITE_WIDTH = 32;

	Player* mPlayer[PLAYERCAP];
	int mNrOfPlayers;
	int mWinner;
	sf::Text mLives[PLAYERCAP];
	sf::Font mFont;

	Block** mBlocks;
	int mNrOfBlocks;
	int mCapBlocks;

	int mNrOfTele;
	sf::Vector2f* mPositionOfTele;

	Pickup** mPickups;
	int mNrOfPickups;
	int mCapPickups;


	// Functions
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void centerRect(sf::FloatRect& rect);
	int  collidePlayer(int iPlayer, sf::FloatRect other);
	void collideFire(Fire* fire);
	void checkCollisionPlayerAndBlocks(int iPlayer);
	void checkCollisionPlayerAndPickups(int iPlayer);
	void checkCollisionPlayerAndBombs(int iPlayer);
	void checkCollisionFireFromPlayer(int iPlayer);
	void checkCollisionFireFromBarrels();
	void findWinner(); // activates on damage to see if game is over 
	void freeMemory();
	void initialize();


	// Pickup related
	void placePickup(sf::Vector2f position);	
	void expandPickup();
	void removePickup(int index);

	// Block related
	void addWall();
	void addBlock(float x, float y);
	void addTele(float x, float y);
	void addCrate(float x, float y);
	void addBarrel(float x, float y);
	void expandBlocks();
	void setTelePos();
	
public:
	Game();
	virtual~Game();
	void update(float dt);

	void reset();
	int getWinner()const; // -1 still playing, player ID if winner
	void startGame(std::string levelDir, int nrOfPlayers);
	
	void loadLevel(std::string levelDir) throw(...);
};
#endif