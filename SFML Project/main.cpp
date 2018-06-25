#include <SFML/Graphics.hpp>
#include <iostream>
#include <crtdbg.h>
#include <ctime>
#include "Game.h"
#include "Menu.h"
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	srand(static_cast<unsigned int>(time(0)));
	sf::RenderWindow window(sf::VideoMode(608, 608), "Bomberman vs project");

	Game game;
	Menu menu;
	bool isPlaying = false;

	int menuOutPut = 0;
	int gameOutPut = 0;
	sf::Clock gameTime;
	float dt;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();

		dt = gameTime.restart().asSeconds();
		if (!isPlaying) // In menu
		{
			menu.update(dt);
			window.draw(menu);
			menuOutPut = menu.getOutPut();
			if (menuOutPut == -1)
			{
				window.close();
			}
			else if (menu.getOutPut() != 0)
			{
				menu.resetOutPut();
				game.startGame("../Resources/Levels/standard.txt", menuOutPut);
				//game.startGame("../Resources/Levels/lab.txt", menuOutPut);
				isPlaying = true;
			}
		}
		else // In game
		{
			game.update(dt);
			window.draw(game);
			gameOutPut = game.getWinner();
			if (gameOutPut == -1)
			{
				// Do nothing
			}
			else 
			{
				isPlaying = false;
				menu.setWinner(gameOutPut);
				game.reset();
			}

		}
		window.display();
	}
}


