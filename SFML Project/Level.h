#ifndef LEVEL_H
#define LEVEL_H
#include <fstream>
#include <string>
#include <iostream>
#include "Graphics.h"
#include "Player.h"
#include "Pickup.h"
#include "Teleporter.h"
#include "Crate.h"
#include "Barrel.h"
class Level : public Graphics
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
	
	void freeMemory();
	void initialize();
	
	void expandPickup();

	// Block related
	void addWall();
	void addBlock(float x, float y);
	void addTele(float x, float y);
	void addCrate(float x, float y);
	void addBarrel(float x, float y);
	void expandBlocks();
	void setTelePos();

public:
	Level();
	virtual~Level();
	void update(float dt);

	void startGame(std::string levelDir, int nrOfPlayers);
	void loadLevel(std::string levelDir) throw(...);
	void reset();
	void updateLifeDisplay(int iPlayer);
	void findWinner(); // activates on damage to see if game is over 
	int getWinner()const; // -1 still playing, player ID if winner

	void teleportPlayer(int iPlayer, Teleporter teleporter);
	void pickupTaken(int iPlayer, int iPickup);
	void removePickup(int iPickup);

	void placePickup(sf::Vector2f position);
	sf::FloatRect getPickupBoundingBox(int iPickup);

	Player* getPlayer(int iPlayer);
	Block* getBlock(int iBlock);
	int getNrOfPlayers() const;
	int getNrOfPickups() const;
	int getNrOfBlocks() const;
};
#endif