#include "Game.h"

Game::Game()
{
}

Game::~Game()
{	
}

void Game::update(float dt)
{	
	mCollision.checkAllCollisions(mLevel);
	mLevel.update(dt);
}

void Game::startGame(std::string levelDir, int nrOfPlayers)
{
	mLevel.startGame(levelDir, nrOfPlayers);
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mLevel, states);
}

int Game::getWinner() const
{
	return mLevel.getWinner();
}

void Game::reset()
{
	mLevel.reset();
}






