#include <iostream>
#include "StartMenu.h"
#include "Game.h"


int main()
{
	//1,2,3,4 difficulty = easy,medium,hard, suomalainen
	short difficulty = 2;
	unsigned windowWidth = 1920;
	unsigned windowHeight = 1080;
	//init rand gen
	std::srand(static_cast<unsigned>(time(NULL)));


	// Game object
	StartMenu* startMenu = new StartMenu(windowWidth, windowHeight);
	while (startMenu->running() && !startMenu->getEndMenu())
	{
		startMenu->update();
		startMenu->render();
	}
	difficulty = startMenu->getDifficulty();
	if (!startMenu->getStartGame())
	{
		delete startMenu;
		return 0;
	}
	delete startMenu;

	Game game(difficulty, windowWidth, windowHeight);


	//Game loop
	while (game.running() && !game.getEndGame())
	{

		game.update();

		//Render
		game.render();
	}



	return 0;
}