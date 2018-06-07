#ifndef MENU_H
#define MENU_H
#include "Game.h"
#include <SFML\Graphics.hpp>
class Menu:public sf::Drawable
{
	enum textType{nrOfPlayers,play, exit, win};
	enum direction { left, up, right, down };
	
private:
	sf::Sprite mBackgroundSprite;
	sf::Texture mBackgroundTex;
	sf::Text mNrOfPlayersText;
	sf::Text mText[4];
	int mSelectedText;
	int mNrOfPlayers;
	sf::Font mFont;
	bool mKeysPressed[4];
	int mOutPut; // -1 = exit, 0 = nothing,  else start game with nrOfPlayers
	int mWinner; // gets praised after victory

	void changeColors();
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
	Menu();
	virtual~Menu();
	void Update(float dt);
	void setWinner(int winner);
	void resetOutPut();
	int getOutPut()const;


};

#endif // !MENU_H

