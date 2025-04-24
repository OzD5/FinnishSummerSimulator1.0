#include "GameManager.h"

GameManager::GameManager(int windowWidth_in, int windowHeight_in, sf::RenderWindow* window):
	windowWidth(windowWidth_in), windowHeight(windowHeight_in), difficulty(0)
{
	this->runStartMenu(this->windowWidth, this->windowHeight, window);

}

void GameManager::changeResoluton(short resolution)
{
	switch (resolution)
	{
	case 1:
		this->windowWidth = 1280;
		this->windowHeight = 720;
		break;
	case 2:
		this->windowWidth = 1920;
		this->windowHeight = 1080;
		break;
	case 3:
		this->windowWidth = 2560;
		this->windowHeight = 1440;
		break;
	default:
		break;
	}
}

void GameManager::runStartMenu(int windowWidth, int windowHeight, sf::RenderWindow* window)
{
	//std::unique_ptr<StartMenu> startMenu = std::make_unique<StartMenu>(windowWidth, windowHeight,window);
	StartMenu* startMenu = new StartMenu(windowWidth, windowHeight, window);
	while (startMenu->running() && !startMenu->getEndMenu())
	{
		startMenu->update();
		startMenu->render();
	}
	startMenu->silenceMusic();
	//If play button is not pressed we close the app. Otherwise we run the game
	if (!startMenu->getStartGame())
	{
		delete startMenu;
		return;
	}
	this->difficulty = startMenu->getDifficulty();
	short resolution = startMenu->getResolution();
	//TODO: RESOLUTION CHANGE NOT WORKING
	changeResoluton(resolution);
	delete startMenu;


	this->runGame(this->difficulty, this->windowWidth, this->windowHeight, window);

}

void GameManager::runGame(short difficulty, int windowWidth, int windowHeight, sf::RenderWindow* window)
{
	//Can't use smartpointers here. They're not so smart afterall
	//std::unique_ptr<Game> game = std::make_unique<Game>(difficulty, windowWidth, windowHeight, window);
	Game* game = new Game(difficulty, windowWidth, windowHeight, window);

	//Game loop
	while (game->running() && !game->getEndGame())
	{

		game->update();

		//Render
		game->render();
	}
	game->silenceMusic();
	game->updateHighScore();
	//After game ends we go back to menu
	delete game;
	this->runStartMenu(this->windowWidth, this->windowHeight, window);
}
