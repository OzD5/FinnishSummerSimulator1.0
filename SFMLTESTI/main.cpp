#include <iostream>
#include "StartMenu.h"
#include "Game.h"


int main()
{
	//1,2,3 difficulty = easy,medium,hard
	short difficulty = 2;
	//init rand gen
	std::srand(static_cast<unsigned>(time(NULL)));

	sf::Clock clock;
	int frameCount = 0;
	float fps = 0.0f; 

	// Game object
	StartMenu* startMenu = new StartMenu();
	while (startMenu->running() && !startMenu->getEndMenu())
	{
		startMenu->update();

		startMenu->render();
	}


	if (!startMenu->getStartGame())
		return 0;
	difficulty = startMenu->getDifficulty();

	delete startMenu;

	Game game(difficulty);

	
	//Game loop
	while (game.running() && !game.getEndGame())
	{

		//Update
		frameCount++;
		sf::Time elapsedTime = clock.getElapsedTime();

		if (elapsedTime.asSeconds() >= 0.1f) {
			fps = static_cast<float>(frameCount) / elapsedTime.asSeconds();
			std::cout << "FPS: " << fps << std::endl;

			// Reset for the next second
			clock.restart();
			frameCount = 0;
		}
		game.update();

		//Render
		game.render();
	}


	return 0;
}