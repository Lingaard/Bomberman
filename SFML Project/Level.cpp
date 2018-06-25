#include "Level.h"

Level::Level()
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

Level::~Level()
{
	freeMemory();
}

void Level::update(float dt)
{
	Barrel* barrelCheck = nullptr;

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

void Level::startGame(std::string levelDir, int nrOfPlayers)
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
	
}

void Level::loadLevel(std::string levelDir) throw(...)
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
	setTelePos();
	file.close();
}

Player * Level::getPlayer(int iPlayer)
{
	return mPlayer[iPlayer];
}

Block * Level::getBlock(int iBlock)
{
	return mBlocks[iBlock];
}

int Level::getNrOfPlayers() const
{
	return mNrOfPlayers;
}

int Level::getNrOfPickups() const
{
	return mNrOfPickups;
}

int Level::getNrOfBlocks() const
{
	return mNrOfBlocks;
}


void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
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

int Level::getWinner() const
{
	return mWinner;
}

void Level::teleportPlayer(int iPlayer, Teleporter teleporter)
{
	mPlayer[iPlayer]->setPosition(teleporter.teleport(mPositionOfTele, mNrOfTele));
	mPlayer[iPlayer]->hasTeleported();
}

void Level::pickupTaken(int iPlayer, int iPickup)
{
	mPlayer[iPlayer]->activatePickup(mPickups[iPickup]->getType());
	removePickup(iPickup);
}

void Level::findWinner()
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

void Level::placePickup(sf::Vector2f position)
{
	if (mNrOfPickups == mCapPickups)
	{
		expandPickup();
	}
	mPickups[mNrOfPickups++] = new Pickup(position);
}

void Level::expandPickup()
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

void Level::removePickup(int index)
{
	delete mPickups[index];
	mPickups[index] = mPickups[--mNrOfPickups];
}

sf::FloatRect Level::getPickupBoundingBox(int iPickup)
{
	return mPickups[iPickup]->getGlobalBounds();
}

void Level::addWall()
{
	for (float i = 0; i < 18; i++)
	{
		addBlock(i, 0);
		addBlock(i + 1, 18.0f);
		addBlock(0, i + 1);
		addBlock(18.0f, i);
	}
}

void Level::addBlock(float x, float y)
{
	if (mNrOfBlocks == mCapBlocks)
	{
		expandBlocks();
	}
	mBlocks[mNrOfBlocks++] = new Block(x*SPRITE_WIDTH, y*SPRITE_WIDTH);
}

void Level::addTele(float x, float y)
{
	if (mNrOfBlocks == mCapBlocks)
	{
		expandBlocks();
	}
	mBlocks[mNrOfBlocks++] = new Teleporter(x*SPRITE_WIDTH, y*SPRITE_WIDTH);
	mNrOfTele++;
}

void Level::addCrate(float x, float y)
{
	if (mNrOfBlocks == mCapBlocks)
	{
		expandBlocks();
	}
	mBlocks[mNrOfBlocks++] = new Crate(x*SPRITE_WIDTH, y*SPRITE_WIDTH);
}

void Level::addBarrel(float x, float y)
{
	if (mNrOfBlocks == mCapBlocks)
	{
		expandBlocks();
	}
	mBlocks[mNrOfBlocks++] = new Barrel(x*SPRITE_WIDTH, y*SPRITE_WIDTH);
}

void Level::expandBlocks()
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

void Level::setTelePos()
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

void Level::reset()
{
	freeMemory();
	initialize();
}

void Level::updateLifeDisplay(int iPlayer)
{
	mLives[iPlayer].setString(std::to_string(mPlayer[iPlayer]->getLives()));
}

void Level::freeMemory()
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

void Level::initialize()
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


