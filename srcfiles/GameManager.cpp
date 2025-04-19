#include "GameManager.h"

GameManager::GameManager(int windowWidth_in, int windowHeight_in):
	windowWidth(windowWidth_in), windowHeight(windowHeight_in), difficulty(0)
{
	this->runStartMenu(this->windowWidth, this->windowHeight);

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

void GameManager::runStartMenu(int windowWidth, int windowHeight)
{
	StartMenu* startMenu = new StartMenu(windowWidth, windowHeight);
	while (startMenu->running() && !startMenu->getEndMenu())
	{
		startMenu->update();
		startMenu->render();
	}
	//If play button is not pressed we close the app. Otherwise we run the game
	if (!startMenu->getStartGame())
	{
		delete startMenu;
		return;
	}
	this->difficulty = startMenu->getDifficulty();
	short resolution = startMenu->getResolution();
	changeResoluton(resolution);

	delete startMenu;

	this->runGame(this->difficulty, this->windowWidth, this->windowHeight);

}

void GameManager::runGame(short difficulty, int windowWidth, int windowHeight)
{
	Game* game = new Game(difficulty, windowWidth, windowHeight);


	//Game loop
	while (game->running() && !game->getEndGame())
	{

		game->update();

		//Render
		game->render();
	}
	delete game;
	//After game ends we go back to menu
	this->runStartMenu(this->windowWidth, this->windowHeight);
}
