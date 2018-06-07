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
	sf::RenderWindow window(sf::VideoMode(608, 608), "SFML works!");
	Game game;
	Menu menu;
	bool isPlaying = false;

	int OutPut = 0;
	sf::Clock gameTime;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();

		if (!isPlaying)
		{
			menu.Update(gameTime.restart().asSeconds());
			window.draw(menu);
			OutPut = menu.getOutPut();
			if (OutPut == -1)
			{
				window.close();
			}
			else if (menu.getOutPut() != 0)
			{
				menu.resetOutPut();
				game.startGame("Levels/standard.txt", OutPut);
				isPlaying = true;
			}
		}
		else
		{
			game.Update(gameTime.restart().asSeconds());
			window.draw(game);
			OutPut = game.getWinner();
			if (OutPut == -1)
			{
				// Do nothing
			}
			else 
			{
				isPlaying = false;
				menu.setWinner(OutPut);
				game.reset();
			}

		}

		window.display();
	}
}


