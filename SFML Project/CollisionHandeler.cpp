#include "CollisionHandeler.h"

void CollisionHandeler::centerRect(sf::FloatRect& rect)
{
	rect.top += rect.height / 2;
	rect.left += rect.width / 2;
	rect.height /= 2;
	rect.width /= 2;
}

int CollisionHandeler::collidePlayer(Player* player, sf::FloatRect other)
{
	int direction;
	sf::FloatRect tempPlayerRect = player->getGlobalBoundsScaled(0.8);
	centerRect(tempPlayerRect);
	centerRect(other);

	float deltaX = tempPlayerRect.left - other.left;
	float deltaY = tempPlayerRect.top - other.top;
	if (abs(deltaX) < abs(deltaY))
	{
		if (deltaY < 0)
		{
			direction = Player::down;
		}
		else
		{
			direction = Player::up;
		}
	}
	else
	{
		if (deltaX < 0)
		{
			direction = Player::right;
		}
		else
		{
			direction = Player::left;
		}
	}
	return direction;
}

void CollisionHandeler::collideFire(Fire * fire, Level& level)
{
	sf::FloatRect tempFireRect = fire->getGlobalBoundsScaled(0.9f); // Scaled slightly to not falsely collide with walls alongside
	Player* player = nullptr;
	Block* block = nullptr;
	Crate* checkCrate = nullptr;
	Barrel* checkBarrel = nullptr;
	

	if (fire->getBurning()) // Deployed and burning
	{
		for (int i2Player = 0; i2Player < level.getNrOfPlayers(); i2Player++)	// Second player layer
		{
			player = level.getPlayer(i2Player);
			if (tempFireRect.intersects((player)->getGlobalBoundsScaled(0.7f))) // Collision with fire and player
			{
				player->takeDamage();
				level.updateLifeDisplay(i2Player);
				level.findWinner();
			}
			for (int i2Bomb = 0; i2Bomb < player->getNrOfBombs(); i2Bomb++)	// Second bomb layer
			{
				if (tempFireRect.intersects(player->getBombGlobalBounds(i2Bomb))) // Collision with fire and bombs
				{
					player->explodeBomb(i2Bomb);
				}
			}
		}
		for (int iPickup = 0; iPickup < level.getNrOfPickups(); iPickup++) // fire and pickups
		{
			if (tempFireRect.intersects(level.getPickupBoundingBox(iPickup)))
			{
				level.removePickup(iPickup);
			}
		}
	}
	else // Deployed not burning
	{
		for (int iBlock = 0; iBlock < level.getNrOfBlocks(); iBlock++) // bomb and blocks
		{
			block = level.getBlock(iBlock);
			if (block->getIsSolid() && tempFireRect.intersects(block->getGlobalBounds()))
			{
				fire->encounterBlock(true);

				checkCrate = dynamic_cast<Crate*>(block);
				if (checkCrate != nullptr)
				{
					// First fire in a direction that collides with a solid block gets set to blockdestroyer.
					// This happens on explosion.
					if (fire->getBlockDestroyer())
					{
						if (checkCrate->getHasItem())
						{
							level.placePickup(checkCrate->getPosition());
						}
						checkCrate->destroy();
					}
				}
				else
				{
					checkBarrel = dynamic_cast<Barrel*>(block);
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

void CollisionHandeler::checkCollisionPlayerAndBlocks(int iPlayer, Level& level)
{
	Teleporter* teleCheck = nullptr;
	Block* block = nullptr;
	Player* player = level.getPlayer(iPlayer);
	player->encounterBlockReset();
	for (int iBlock = 0; iBlock < level.getNrOfBlocks(); iBlock++) // Player and block
	{
		block = level.getBlock(iBlock);
		if (player->getGlobalBoundsScaled().intersects(block->getGlobalBounds()))
		{
			if (block->getIsSolid())
			{
				int direction = collidePlayer(player, block->getGlobalBounds());
				player->encounterBlock(direction);
			}
			else
			{
				teleCheck = dynamic_cast<Teleporter*>(block);
				if (teleCheck != nullptr && player->canTeleport())
				{
					level.teleportPlayer(iPlayer, *teleCheck);
				}
			}
		}
	}
}

void CollisionHandeler::checkCollisionPlayerAndPickups(int iPlayer, Level& level)
{
	for (int iPickup = 0; iPickup < level.getNrOfPickups(); iPickup++)
	{
		if (level.getPlayer(iPlayer)->getGlobalBoundsScaled().intersects(level.getPickupBoundingBox(iPickup)))
		{
			level.pickupTaken(iPlayer, iPickup);
		}
	}
}

void CollisionHandeler::checkCollisionPlayerAndBombs(int iPlayer, Level& level)
{
	Player* player = level.getPlayer(iPlayer);
	Player* player2 = nullptr;
	for (int i2Player = 0; i2Player < level.getNrOfPlayers(); i2Player++)
	{
		player2 = level.getPlayer(i2Player);
		for (int i2Bomb = 0; i2Bomb < player2->getNrOfBombs(); i2Bomb++)
		{
			int direction;
			if (player->getGlobalBoundsScaled().intersects(player2->getBombGlobalBounds(i2Bomb)))
			{
				direction = collidePlayer(player, player2->getBombGlobalBounds(i2Bomb));

				// Only collide if outside of bound collides. Prevents collision with bombs just placed.
				if (!player->getGlobalBoundsScaled(0.6f).intersects(player2->getBombGlobalBounds(i2Bomb)))
					player->encounterBlock(direction);
			}
		}
	}
}

void CollisionHandeler::checkCollisionFireFromPlayer(int iPlayer, Level& level)
{
	Player* player = level.getPlayer(iPlayer);
	for (int iBomb = 0; iBomb < player->getNrOfBombs(); iBomb++) // Bomb layer
	{
		if (player->getIsFireDeployed(iBomb, 0))
		{
			for (int iFire = 0; iFire < player->getNrOfFires(); iFire++) // Fire layer. 
			{
				collideFire(player->getFire(iBomb, iFire), level);
			}
		}
	}
}

void CollisionHandeler::checkCollisionFireFromBarrels(Level& level)
{
	Barrel* barrelCheck = nullptr;
	for (int iBarrel = 0; iBarrel < level.getNrOfBlocks(); iBarrel++)
	{
		barrelCheck = dynamic_cast<Barrel*>(level.getBlock(iBarrel));
		if (barrelCheck != nullptr)
		{
			if (barrelCheck->getFire(0)->getDeployed())
			{
				for (int iFire = 0; iFire < barrelCheck->getNrOfFires(); iFire++)
				{
					collideFire(barrelCheck->getFire(iFire), level);
				}
			}
		}
	}
}

CollisionHandeler::CollisionHandeler()
{
}

CollisionHandeler::~CollisionHandeler()
{
}

void CollisionHandeler::checkAllCollisions(Level & level)
{
	// Check collisions
	for (int iPlayer = 0; iPlayer < level.getNrOfPlayers(); iPlayer++)
	{
		checkCollisionPlayerAndBlocks(iPlayer, level);
		checkCollisionPlayerAndPickups(iPlayer, level);
		checkCollisionPlayerAndBombs(iPlayer, level);
		checkCollisionFireFromPlayer(iPlayer, level);
	}
	checkCollisionFireFromBarrels(level);
}
