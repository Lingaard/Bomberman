#ifndef MENU_H
#define MENU_H
#include "Graphics.h"

class Menu:public Graphics
{
	enum textType{nrOfPlayers,play, exit, win};
	enum direction { left, up, right, down };
	
private:
	sf::Font mFont;
	sf::Text mText[4];
	bool mKeysPressed[4]; // Used to get only one action per key press
	int mSelectedText;
	int mNrOfPlayers;
	int mOutPut; // -1 = exit, 0 = nothing,  else start game with nrOfPlayers
	int mWinner; // gets praised after victory

	void changeColors();
	void moveBackground(float dt);
	void Menu::draw(sf::RenderTarget & target, sf::RenderStates states) const;
public:
	Menu();
	virtual~Menu();
	void update(float dt);
	void setWinner(int winner);
	void resetOutPut();
	int getOutPut()const;
};

#endif // !MENU_H