#include "Game.h"
#include <iostream>

const enum direction { left, up, right, down };

Game::Game()
{
	if (mBackgroundTex.loadFromFile("../Resources/background.jpg"))
	{
		// Handle error
	}
	mBackgroundSprite.setTexture(mBackgroundTex);	

	initialize();
}

Game::~Game()
{
	freeMemory();
}

void Game::Update(float dt)
{
	Teleporter* teleCheck = nullptr;
	Barrel* barrelCheck = nullptr;

	// Check collisions
	for (int iPlayer = 0; iPlayer < mNrOfPlayers; iPlayer++) // Player layer
	{
		/*
		Check collision with boxes. read their bounding box and determine which direction the box is to the player 
		stop the player from going to that direction. 
		Maybe implement a stop direction whith enum directions left top and so on.		
		*/
		mPlayer[iPlayer]->encounterBlockReset();
		for (int iBlock = 0; iBlock < mNrOfBlocks; iBlock++) // Player and block
		{
			if (mPlayer[iPlayer]->getPlayerGlobalBounds().intersects(mBlocks[iBlock]->getGlobalBounds()))
			{
				if(mBlocks[iBlock]->getIsSolid())
				{
					int direction =	collidePlayer(iPlayer, mBlocks[iBlock]->getGlobalBounds());
					mPlayer[iPlayer]->encounterBlock(direction);
				}
				else
				{
					teleCheck = dynamic_cast<Teleporter*>(mBlocks[iBlock]);
					if (teleCheck != nullptr && mPlayer[iPlayer]->canTeleport())
					{
						teleCheck->teleport(mPlayer[iPlayer]->getSprite(), mPositionOfTele, mNrOfTele, mBlocks[iBlock]->getPosition());
						mPlayer[iPlayer]->hasTeleported();
					}
				}
			}
		}


		
		for (int iPickup = 0; iPickup < mNrOfPickups; iPickup++) // Player and pickup
		{
			if (mPlayer[iPlayer]->getPlayerGlobalBounds().intersects(mPickups[iPickup]->getGlobalBounds()))
			{
				mPlayer[iPlayer]->activatePickup(mPickups[iPickup]->getType());
				removePickup(iPickup);
			}
		}


		for (int i2Player = 0; i2Player < mNrOfPlayers; i2Player++)
		{
			for (int i2Bomb = 0; i2Bomb < mPlayer[i2Player]->getNrOfBombs(); i2Bomb++)
			{
				int direction;
				if (mPlayer[i2Player]->getBombGlobalBounds(i2Bomb).contains(mPlayer[iPlayer]->getSprite().getPosition()))
				{
					// Do nothing
				}
				else if (mPlayer[iPlayer]->getPlayerGlobalBounds().intersects(mPlayer[i2Player]->getBombGlobalBounds(i2Bomb)))
				{
					direction = collidePlayer(iPlayer, mPlayer[i2Player]->getBombGlobalBounds(i2Bomb));

					if(!mPlayer[iPlayer]->getPlayerGlobalBounds(0.6).intersects(mPlayer[i2Player]->getBombGlobalBounds(i2Bomb)))
						mPlayer[iPlayer]->encounterBlock(direction);


				
				}
			}
		}

		

		for (int iBomb = 0; iBomb < mPlayer[iPlayer]->getNrOfBombs(); iBomb++) // Bomb layer
		{
			
			if (mPlayer[iPlayer]->getIsFireDeployed(iBomb, 0))  
			{
				for (int iFire = 0; iFire < mPlayer[iPlayer]->getNrOfFires(); iFire++) // Fire layer. 
				{
					collideFire(mPlayer[iPlayer]->getFire(iBomb, iFire));
				}
			}			
		}
	}

	for (int iBarrel = 0; iBarrel < mNrOfBlocks; iBarrel++)	//Check collisions for fire from Barrels
	{
		barrelCheck = dynamic_cast<Barrel*>(mBlocks[iBarrel]);
		if (barrelCheck != nullptr)
		{
			if(barrelCheck->getFire(0)->getDeployed())
			for (int iFire = 0; iFire < barrelCheck->getNrOfFires(); iFire++)
			{
				collideFire(barrelCheck->getFire(iFire));
			}
		}
	}


	// Update everything
	for (int i = 0; i < mNrOfPlayers; i++)
	{
		mPlayer[i]->Update(dt);
	}
	for (int i = 0; i < mNrOfBlocks; i++) 
	{
		barrelCheck = dynamic_cast<Barrel*>(mBlocks[i]);
		if (barrelCheck != nullptr)
		{
			barrelCheck->Update(dt);
		}
	}
	std::cout << std::floor(1 / dt) << std::endl;
	
}

void Game::startGame(std::string levelDir, int nrOfPlayers)
{
	mNrOfPlayers = nrOfPlayers;
	for (int i = 0; i<mNrOfPlayers; i++)
		mPlayer[i] = new Player(i+1, i + 1, sf::Vector2f(32 + 32 * 16 * (i / 2), 32 + 32 * 16 * (i % 2)));

	try
	{
		loadLevel(levelDir);

	}
	catch (const char* e)
	{
		std::cout << e;
	}
	setTelePos();
}

void Game::loadLevel(std::string levelDir) throw(...)
{
	enum BlockType { empty, block, crate, teleporter, barrel };
	int square = empty;
	std::ifstream file;
	file.open(levelDir);
	if (!file.is_open())
	{
		throw "Could not load level.";
	}
	
	addWall();
	
	for (int y = 1; y < 18; y++)
	{
		for (int x = 1; x < 18; x++)
		{
			file >> square;
			switch (square)
			{
			case empty:
				//Do nothing.
				break;
			case block:
				addBlock(x, y);
				break;
			case crate:
				addCrate(x, y);
				break;
			case teleporter:
				addTele(x, y);
				break;
			case barrel:
				addBarrel(x, y);
				break;
			}
		}
	}
	file.close();

}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// Make sure everything in the game is drawn.


	target.draw(mBackgroundSprite, states);
	for (int i = 0; i < mNrOfBlocks; i++)
	{
		target.draw(*mBlocks[i], states);
	}

	for (int i = 0; i < mNrOfPickups; i++)
	{
		target.draw(*mPickups[i], states);
	}

	for (int i = 0; i < mNrOfPlayers; i++)
	{
		target.draw(*mPlayer[i], states);
	}


}

void Game::centerRect(sf::FloatRect& rect)
{
	rect.top += rect.height / 2;
	rect.left += rect.width / 2;
	rect.height/= 2;
	rect.width /= 2;
}

int Game::collidePlayer(int iPlayer, sf::FloatRect other)
{
	int direction;
	sf::FloatRect tempPlayerRect = mPlayer[iPlayer]->getPlayerGlobalBounds();
	centerRect(tempPlayerRect);
	centerRect(other);

	float deltaX = tempPlayerRect.left - other.left;
	float deltaY = tempPlayerRect.top - other.top;
	if (abs(deltaX) < abs(deltaY))
	{
		if (deltaY < 0)
		{
			direction =Player::down;			
		}
		else
		{
			direction =Player::up;
		}
	}
	else
	{
		if (deltaX < 0)
		{
			direction =Player::right;
		}
		else
		{
			direction =Player::left;
		}
	}
	/*mPlayer[iPlayer]->encounterBlock(direction);*/
	return direction;
}

void Game::collideFire(Fire * fire)
{
	sf::FloatRect tempFireRect = fire->getGlobalBounds();
	Crate* checkCrate = nullptr;
	Barrel* checkBarrel = nullptr;


	if (fire->getBurning()) // Deployed and burning
	{
		for (int i2Player = 0; i2Player < mNrOfPlayers; i2Player++)	// Second player layer
		{
			if (tempFireRect.intersects(mPlayer[i2Player]->getPlayerGlobalBounds())) // Collision with fire and player
			{
				mPlayer[i2Player]->takeDamage();
				findWinner();
			}
			for (int i2Bomb = 0; i2Bomb < mPlayer[i2Player]->getNrOfBombs(); i2Bomb++)	// Second bomb layer
			{
				if (tempFireRect.intersects(mPlayer[i2Player]->getBombGlobalBounds(i2Bomb))) // Collision with fire and bombs
				{
					mPlayer[i2Player]->explodeBomb(i2Bomb);
				}
			}
		}
		for (int iPickup = 0; iPickup < mNrOfPickups; iPickup++) // fire and pickups
		{
			if (tempFireRect.intersects(mPickups[iPickup]->getGlobalBounds()))
			{
				removePickup(iPickup);
			}
		}
	}
	else // Deployed not burning
	{
		for (int iBlock = 0; iBlock < mNrOfBlocks; iBlock++) // bomb and blocks
		{

			if (mBlocks[iBlock]->getIsSolid() && tempFireRect.intersects(mBlocks[iBlock]->getGlobalBounds()))
			{				
				fire->encounterBlock(true);

				checkCrate = dynamic_cast<Crate*>(mBlocks[iBlock]);
				
				if (checkCrate != nullptr)
				{
					if (fire->getBlockDestroyer())
					{
						if (checkCrate->getHasItem())
						{
							placePickup(checkCrate->getPosition());
						}
						checkCrate->destroy();
					}
				}
				else
				{
					checkBarrel = dynamic_cast<Barrel*>(mBlocks[iBlock]);
					if (checkBarrel != nullptr)
					{
						if (fire->getBlockDestroyer())
						{
							checkBarrel->explode();
						}
					}
				}

			}
		}
	}
}

int Game::getWinner() const
{
	return mWinner;
}

void Game::findWinner()
{
	int nrAlive = 0;
	for (int i = 0; i < mNrOfPlayers; i++)
	{
		if (mPlayer[i]->isAlive())
			nrAlive++;
	}
	if (nrAlive > 1)
	{
		//Do nothing
	}
	else if (nrAlive == 1)
	{
		for (int i = 0; i < mNrOfPlayers; i++)
		{
			if (mPlayer[i]->isAlive())
				mWinner = i;
		}
	}
	else // if under 1 alive (Maybe could happen with double knockout?)
	{
		mWinner = -2;
	}
}



void Game::placePickup(sf::Vector2f position)
{
	if (mNrOfPickups == mCapPickups)
	{
		expandPickup();
	}
	mPickups[mNrOfPickups++] = new Pickup(position);
}

void Game::expandPickup()
{
	mCapPickups += 5;
	Pickup** temp = new Pickup*[mCapPickups];
	for (int i = 0; i < mNrOfPickups; i++)
	{
		temp[i] = mPickups[i];
	}
	for (int i = mNrOfPickups; i < mCapPickups; i++)
	{
		temp[i] = nullptr;
	}
	delete[]mPickups;
	mPickups = temp;
}

void Game::removePickup(int index)
{
	delete mPickups[index];
	mPickups[index] = mPickups[--mNrOfPickups];
}



void Game::addWall()
{
	for (int i = 0; i < 18; i++)
	{
		addBlock(i,  0);
		addBlock(i+1,  18);
		addBlock(0,  i+1);
		addBlock(18, i);
	}
	
}

void Game::addBlock(float x, float y)
{
	if (mNrOfBlocks == mCapBlocks)
	{
		expandBlocks();
	}

	mBlocks[mNrOfBlocks++] = new Block(x*SPRITE_WIDTH, y*SPRITE_WIDTH);
}

void Game::addTele(float x, float y)
{
	if (mNrOfBlocks == mCapBlocks)
	{
		expandBlocks();
	}
	mBlocks[mNrOfBlocks++] = new Teleporter(x*SPRITE_WIDTH, y*SPRITE_WIDTH);
	mNrOfTele++;
}

void Game::addCrate(float x, float y)
{
	if (mNrOfBlocks == mCapBlocks)
	{
		expandBlocks();
	}
	mBlocks[mNrOfBlocks++] = new Crate(x*SPRITE_WIDTH, y*SPRITE_WIDTH);
}

void Game::addBarrel(float x, float y)
{
	if (mNrOfBlocks == mCapBlocks)
	{
		expandBlocks();
	}
	mBlocks[mNrOfBlocks++] = new Barrel(x*SPRITE_WIDTH, y*SPRITE_WIDTH);
}

void Game::expandBlocks()
{
	mCapBlocks += 30;
	Block** temp = new Block*[mCapBlocks];
	for (int i = 0; i < mNrOfBlocks; i++)
	{
		temp[i] = mBlocks[i];
	}
	for (int i = mNrOfBlocks; i < mCapBlocks; i++)
	{
		temp[i] = nullptr;
	}
	delete[]mBlocks;
	mBlocks = temp;
}

void Game::setTelePos()
{
	Teleporter* teleCheck = nullptr;
	mPositionOfTele = new sf::Vector2f[mNrOfTele];
	for (int i = 0, iTeles = 0; iTeles < mNrOfTele; i++)
	{
		teleCheck = dynamic_cast<Teleporter*>(mBlocks[i]);
		if (teleCheck != nullptr)
		{
			mPositionOfTele[iTeles++] = sf::Vector2f(teleCheck->getPosition());
		}
	}
}

void Game::reset()
{
	freeMemory();
	initialize();
}

void Game::freeMemory()
{
	for (int i = 0; i < mNrOfPlayers; i++)
	{
		delete mPlayer[i];
	}
	for (int i = 0; i < mNrOfBlocks; i++)
	{
		delete mBlocks[i];
	}
	delete[]mBlocks;
	mBlocks = nullptr;
	delete[]mPositionOfTele;
	mBlocks = nullptr;
	for (int i = 0; i < mNrOfPickups; i++)
	{
		delete mPickups[i];
	}
	delete[]mPickups;
	mPickups = nullptr;
}

void Game::initialize()
{
	mCapPickups = 8;
	mCapBlocks = 120;

	mNrOfBlocks = 0;
	mNrOfTele = 0;
	mNrOfPickups = 0;

	mNrOfPlayers = 0;
	mWinner = -1;

	mPickups = new Pickup*[mCapPickups];
	mBlocks = new Block*[mCapBlocks];
	mPositionOfTele = nullptr;
}
