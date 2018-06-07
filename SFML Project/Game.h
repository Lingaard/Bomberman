#ifndef GAME_H
#define GAME_H
#include <SFML\Graphics.hpp>
#include <fstream>
#include <string>
#include "Player.h"
#include "Pickup.h"
#include "Teleporter.h"
#include "Crate.h"
#include "Barrel.h"
class Game : public sf::Drawable
{
private:
	static const int PLAYERCAP = 4;
	static const int SPRITE_WIDTH = 32;

	Player* mPlayer[PLAYERCAP];
	int mNrOfPlayers;
	int mWinner;

	Block** mBlocks;
	int mNrOfBlocks;
	int mCapBlocks;


	int mNrOfTele;
	sf::Vector2f* mPositionOfTele;

	Pickup** mPickups;
	int mNrOfPickups;
	int mCapPickups;
	

	sf::Texture mBackgroundTex;
	sf::Sprite mBackgroundSprite;

	// Functions
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void centerRect(sf::FloatRect& rect);
	int collidePlayer(int iPlayer, sf::FloatRect other);
	void collideFire(Fire* fire);
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
	void expandBarrels();
	void setTelePos();


public:
	Game();
	virtual~Game();
	void Update(float dt);

	void reset();
	int getWinner()const; // -1 still playing, player ID if winner
	void startGame(std::string levelDir, int nrOfPlayers);
	
	void loadLevel(std::string levelDir) throw(...);
};
#endif