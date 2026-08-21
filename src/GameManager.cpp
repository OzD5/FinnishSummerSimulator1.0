#include "GameManager.hpp"
#include <array>
#include <utility>
#include <tuple>
#include <algorithm>

GameManager::GameManager(int windowWidth_in, int windowHeight_in, sf::RenderWindow* window):
	windowWidth(windowWidth_in), windowHeight(windowHeight_in), window(window), difficulty(0)
{

}

void GameManager::run()
{
	using enum GameChoice;
	GameChoice currentTask = RunStartMenu;
	while (currentTask != Quit) {
		if (currentTask == RunStartMenu) {
			currentTask = runStartMenu();
		}	
		else {
			currentTask = runGame();
		}
	}
}

void GameManager::changeResoluton(const short resolution)
{
	std::array<std::pair<int,int>, 3> windowSizes = {{{1280, 720}, {1920, 1080}, {2560, 1440}}};

	// Resolution is either 1, 2 or 3.
	auto resolutionChoice = std::clamp(resolution - 1, 0, 2);
	std::tie(this->windowWidth, this->windowHeight) = windowSizes[resolutionChoice]; 

	delete window;
	this->window = new sf::RenderWindow(sf::VideoMode(this->windowWidth, this->windowHeight), "Finnish Summer Simulator", sf::Style::Titlebar | sf::Style::Default);
}

GameChoice GameManager::runStartMenu()
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
		return GameChoice::Quit;
	}
	this->difficulty = startMenu->getDifficulty();
	short resolution = startMenu->getResolution();

	changeResoluton(resolution);
	delete startMenu;

	return GameChoice::RunGame;
}

GameChoice GameManager::runGame()
{
	Game* game = new Game(difficulty, windowWidth, windowHeight, window);
	//PauseMenu* pauseMenu = new PauseMenu(window, windowWidth, windowHeight);
	//Game loop
	while (game->running() && !game->getEndGame() /* Only while pausemenu not implemented */&& !game->getPauseMenu())
	{
		if (!game->getPauseMenu()) {

			game->update();
			game->render();
		}
		else {
			//pauseMenu->update();
			//pauseMenu->render();
		}

	}
	//Saving progress and running death menu
	game->onGameEnd();

	//After game ends we go back to menu
	delete game;
	return GameChoice::RunStartMenu;
}