#include "Menu.h"
#include "iostream"


void Menu::changeColors()
{
	for (int i = 0; i < 3; i++)
	{
		if (i == mSelectedText)
		{
			mText[i].setColor(sf::Color::Red);
		}
		else
		{
			mText[i].setColor(sf::Color::White);
		}
	}
}

void Menu::moveBackground(float dt)
{
	mBackgroundSprite.move(-60 * dt, 100 * dt);

	if (mBackgroundSprite.getPosition().y > 0)
	{
		mBackgroundSprite.move(0, -32);
	}
	if (mBackgroundSprite.getPosition().x < -32)
	{
		mBackgroundSprite.move(32, 0);
	}
}

void Menu::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mBackgroundSprite, states);
	for (int i = 0; i < 4; i++)
	{
		target.draw(mText[i],states);
	}
}

Menu::Menu()
{
	mNrOfPlayers = 2;
	mOutPut = 0;
	mWinner = -1;
	mBackgroundTex.loadFromFile("../Resources/background scrolling.jpg");
	mBackgroundSprite.setTexture(mBackgroundTex);
	mBackgroundSprite.setPosition(0, -32);
	mFont.loadFromFile("../Resources/VCR_OSD_MONO_1.001.ttf");
	for (int i = 0; i < 4; i++)
	{
		mText[i].setCharacterSize(40);
		mText[i].setPosition(250, 250 + i * 50);
		mText[i].setFont(mFont);
	}
	mText[nrOfPlayers].setPosition(80, 250);
	mText[win].setPosition(20, 100);
	mText[win].setCharacterSize(55);
	mText[win].setColor(sf::Color::Yellow);

	mText[nrOfPlayers].setString("Number of players: " + std::to_string(mNrOfPlayers));
	mText[play].setString("Play");
	mText[exit].setString("Exit");


	for(int i = 0;i<4;i++)
	{
		mKeysPressed[i] = false;
	}
	mSelectedText = textType::nrOfPlayers;
	changeColors();
}

Menu::~Menu()
{
}

void Menu::update(float dt)
{
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		mKeysPressed[direction::up] = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		mKeysPressed[direction::down] = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		mKeysPressed[direction::left] = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		mKeysPressed[direction::right] = false;

	// Get input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) &&
		!mKeysPressed[direction::up])
	{
		--mSelectedText += 3;
		mSelectedText %= 3;
		changeColors();
		mKeysPressed[direction::up] = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) &&
		!mKeysPressed[direction::down])
	{
		++mSelectedText %= 3;
		changeColors();
		mKeysPressed[direction::down] = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) &&
		!mKeysPressed[direction::left] && mSelectedText == textType::nrOfPlayers)
	{
		if (mNrOfPlayers > 2)
		{
			mNrOfPlayers--;
			mText[nrOfPlayers].setString("Number of players: " + std::to_string(mNrOfPlayers));
		}
		mKeysPressed[direction::left] = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
		!mKeysPressed[direction::right] && mSelectedText == textType::nrOfPlayers)
	{
		if (mNrOfPlayers < 4)
		{
			mNrOfPlayers++;
			mText[nrOfPlayers].setString("Number of players: " + std::to_string(mNrOfPlayers));
		}
		mKeysPressed[direction::right] = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
	{
		if (mSelectedText == play)
			mOutPut = mNrOfPlayers;
		else if (mSelectedText == exit)
			mOutPut = -1;
	}

	moveBackground(dt);
}

void Menu::setWinner(int winner)
{
	switch (winner)
	{
	case 0:
		mText[win].setColor(sf::Color::Blue);
		break;
	case 1:
		mText[win].setColor(sf::Color::Red);
		break;
	case 2:
		mText[win].setColor(sf::Color::Green);
		break;
	case 3:
		mText[win].setColor(sf::Color::Cyan);
		break;
	default:
		mText[win].setColor(sf::Color::Yellow);
	}

	if(winner != -2) // Game sets -2 for tie
		mText[win].setString("Winner is player " + std::to_string(winner + 1));
	else 
		mText[win].setString("There was a tie!");
}

void Menu::resetOutPut()
{
	mOutPut = 0;
}

int Menu::getOutPut() const
{
	return mOutPut;
}
