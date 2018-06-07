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
	//font.loadFromFile("../Resources/VCR_OSD_MONO_1.001.ttf");
	mFont.loadFromFile("../Resources/OptimusPrinceps.ttf");
	for (int i = 0; i < 4; i++)
	{
		mText[i].setCharacterSize(40);
		mText[i].setPosition(150, 250 + i * 50);
		mText[i].setFont(mFont);
	}
	mText[win].setPosition(50, 100);
	mText[win].setCharacterSize(60);
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

void Menu::Update(float dt)
{
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
		!mKeysPressed[direction::left] && mSelectedText==textType::nrOfPlayers)
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

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		mKeysPressed[direction::up] = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		mKeysPressed[direction::down] = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		mKeysPressed[direction::left] = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		mKeysPressed[direction::right] = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
	{
		if (mSelectedText == play)
			mOutPut = mNrOfPlayers;
		else if (mSelectedText == exit)
			mOutPut = -1;
	}

	// Move background
	mBackgroundSprite.move(0.0f, -100 * dt);
	if (mBackgroundSprite.getPosition().y < -608)
	{
		mBackgroundSprite.setPosition(0.0f, 0.0f);
	}

}

void Menu::setWinner(int winner)
{
	if(winner != -2)
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
