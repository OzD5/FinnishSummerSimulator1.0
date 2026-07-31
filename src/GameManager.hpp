#pragma once
#include "Game.hpp"
#include "PauseMenu.hpp"
#include "StartMenu.hpp"

enum class GameChoice {Quit, RunStartMenu, RunGame};
class GameManager
{
private:
	int windowWidth;
	int windowHeight;
	sf::RenderWindow* window;
	short difficulty;
	void changeResoluton(const short resolution);
	GameChoice runStartMenu();
	GameChoice runGame();
public:
	explicit GameManager(int windowWidth_in, int windowHeight_in, sf::RenderWindow* window);
	void run();
};