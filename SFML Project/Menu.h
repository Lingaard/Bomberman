#ifndef MENU_H
#define MENU_H
#include <SFML\Graphics.hpp>

class Menu:public sf::Drawable
{
	enum textType{nrOfPlayers,play, exit, win};
	enum direction { left, up, right, down };
	
private:
	sf::Sprite mBackgroundSprite;
	sf::Texture mBackgroundTex;
	sf::Font mFont;
	sf::Text mText[4];
	bool mKeysPressed[4]; // Used to get only one action per key press
	int mSelectedText;
	int mNrOfPlayers;
	int mOutPut; // -1 = exit, 0 = nothing,  else start game with nrOfPlayers
	int mWinner; // gets praised after victory

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void changeColors();
	void moveBackground(float dt);

public:
	Menu();
	virtual~Menu();
	void update(float dt);
	void setWinner(int winner);
	void resetOutPut();
	int getOutPut()const;
};

#endif // !MENU_H