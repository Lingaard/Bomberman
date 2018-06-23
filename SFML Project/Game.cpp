#include "Game.h"

Game::Game()
{
	setTexture("../Resources/background.jpg");

	mFont.loadFromFile("../Resources/VCR_OSD_MONO_1.001.ttf");
	for (int i = 0, pos = 0; i < 4; i++, pos += 3, pos %= 4)
	{
		mLives[i].setCharacterSize(28);
		mLives[i].setPosition(32.0f * 18 * (pos / 2) + 10.0f, 32.0f * 18 * (pos % 2) - 3.0f);
		mLives[i].setFont(mFont);
	}
	mLives[0].setColor(sf::Color::Blue);
	mLives[1].setColor(sf::Color::Red);
	mLives[2].setColor(sf::Color::Green);
	mLives[3].setColor(sf::Color::Cyan);

	initialize();
}

Game::~Game()
{
	freeMemory();
}

void Game::update(float dt)
{	
	Barrel* barrelCheck = nullptr;

	// Check collisions
	for (int iPlayer = 0; iPlayer < mNrOfPlayers; iPlayer++) 
	{
		checkCollisionPlayerAndBlocks(iPlayer);		
		checkCollisionPlayerAndPickups(iPlayer);
		checkCollisionPlayerAndBombs(iPlayer);	
		checkCollisionFireFromPlayer(iPlayer);
	}
	checkCollisionFireFromBarrels();

	// Update everything
	for (int i = 0; i < mNrOfPlayers; i++)
	{
		mPlayer[i]->update(dt);
	}
	for (int i = 0; i < mNrOfBlocks; i++) 
	{
		barrelCheck = dynamic_cast<Barrel*>(mBlocks[i]);
		if (barrelCheck != nullptr)
		{
			barrelCheck->update(dt);
		}
	}	
}

void Game::startGame(std::string levelDir, int nrOfPlayers)
{
	mNrOfPlayers = nrOfPlayers;
	// Places players around the corners.
	for (int i = 0, pos = 0; i < mNrOfPlayers; i++, pos += 3, pos %= 4)
	{
		mPlayer[i] = new Player(i + 1, i + 1, sf::Vector2f(32.0f + 32.0f * 16 * (pos / 2), 32.0f + 32.0f * 16 * (pos % 2)));
		mLives[i].setString(std::to_string(mPlayer[i]->getLives()));
	}

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
	std::ifstream file;
	file.open(levelDir);
	if (!file.is_open())
	{
		throw "Could not load level.";
	}
	enum BlockType { empty, block, crate, teleporter, barrel };
	int square = empty;
	
	addWall();
	
	for (float y = 1; y < 18; y++)
	{
		for (float x = 1; x < 18; x++)
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
	target.draw(getSprite(), states);
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
		target.draw(mLives[i]);
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
	sf::FloatRect tempPlayerRect = mPlayer[iPlayer]->getGlobalBounds();
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
	return direction;
}

void Game::collideFire(Fire * fire)
{
	sf::FloatRect tempFireRect = fire->getGlobalBoundsScaled(0.9f); // Scaled slightly to not falsely collide with walls alongside
	Crate* checkCrate = nullptr;
	Barrel* checkBarrel = nullptr;


	if (fire->getBurning()) // Deployed and burning
	{
		for (int i2Player = 0; i2Player < mNrOfPlayers; i2Player++)	// Second player layer
		{
			if (tempFireRect.intersects(mPlayer[i2Player]->getGlobalBoundsScaled(0.7f))) // Collision with fire and player
			{
				mPlayer[i2Player]->takeDamage();
				mLives[i2Player].setString(std::to_string(mPlayer[i2Player]->getLives()));
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
					// First fire in a direction that collides with a solid block gets set to blockdestroyer.
					// This happens on explosion.
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
		// Do nothing
	}
	else if (nrAlive == 1)
	{
		for (int i = 0; i < mNrOfPlayers; i++)
		{
			if (mPlayer[i]->isAlive())
				mWinner = i;
		}
	}
	else // If tie 
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
	for (float i = 0; i < 18; i++)
	{
		addBlock(i,  0);
		addBlock(i + 1, 18.0f);
		addBlock(0, i + 1);
		addBlock(18.0f, i);
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

void Game::checkCollisionPlayerAndBlocks(int iPlayer)
{
	Teleporter* teleCheck = nullptr;
	mPlayer[iPlayer]->encounterBlockReset();
	for (int iBlock = 0; iBlock < mNrOfBlocks; iBlock++) // Player and block
	{
		if (mPlayer[iPlayer]->getGlobalBoundsScaled().intersects(mBlocks[iBlock]->getGlobalBounds()))
		{
			if (mBlocks[iBlock]->getIsSolid())
			{
				int direction = collidePlayer(iPlayer, mBlocks[iBlock]->getGlobalBounds());
				mPlayer[iPlayer]->encounterBlock(direction);
			}
			else
			{
				teleCheck = dynamic_cast<Teleporter*>(mBlocks[iBlock]);
				if (teleCheck != nullptr && mPlayer[iPlayer]->canTeleport())
				{					
					mPlayer[iPlayer]->setPosition(teleCheck->teleport(mPositionOfTele, mNrOfTele));
					mPlayer[iPlayer]->hasTeleported();
				}
			}
		}
	}
}

void Game::checkCollisionPlayerAndPickups(int iPlayer)
{
	for (int iPickup = 0; iPickup < mNrOfPickups; iPickup++)
	{
		if (mPlayer[iPlayer]->getGlobalBoundsScaled().intersects(mPickups[iPickup]->getGlobalBounds()))
		{
			mPlayer[iPlayer]->activatePickup(mPickups[iPickup]->getType());
			removePickup(iPickup);
		}
	}
}

void Game::checkCollisionPlayerAndBombs(int iPlayer)
{
	for (int i2Player = 0; i2Player < mNrOfPlayers; i2Player++)
	{
		for (int i2Bomb = 0; i2Bomb < mPlayer[i2Player]->getNrOfBombs(); i2Bomb++)
		{
			int direction;
			if (mPlayer[iPlayer]->getGlobalBoundsScaled().intersects(mPlayer[i2Player]->getBombGlobalBounds(i2Bomb)))
			{
				direction = collidePlayer(iPlayer, mPlayer[i2Player]->getBombGlobalBounds(i2Bomb));

				// Only collide if outside of bound collides. Prevents collision with bombs just placed.
				if (!mPlayer[iPlayer]->getGlobalBoundsScaled(0.6f).intersects(mPlayer[i2Player]->getBombGlobalBounds(i2Bomb)))
					mPlayer[iPlayer]->encounterBlock(direction);
			}
		}
	}
}

void Game::checkCollisionFireFromPlayer(int iPlayer)
{
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

void Game::checkCollisionFireFromBarrels()
{
	Barrel* barrelCheck = nullptr;
	for (int iBarrel = 0; iBarrel < mNrOfBlocks; iBarrel++)
	{
		barrelCheck = dynamic_cast<Barrel*>(mBlocks[iBarrel]);
		if (barrelCheck != nullptr)
		{
			if (barrelCheck->getFire(0)->getDeployed())
			{
				for (int iFire = 0; iFire < barrelCheck->getNrOfFires(); iFire++)
				{
					collideFire(barrelCheck->getFire(iFire));
				}
			}
		}
	}
}
