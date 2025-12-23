#pragma once
#include "Game.h"
#include "PauseMenu.h"
#include "StartMenu.h"

class GameManager
{
private:
	int windowWidth;
	int windowHeight;
	short difficulty;
	void changeResoluton(const short& resolution, sf::RenderWindow* window);
	void runStartMenu(int windowWidth, int windowHeight, sf::RenderWindow* window);
	void runGame(const short& difficulty, const int& windowWidth, const int& windowHeight, sf::RenderWindow* window);
public:
	explicit GameManager(int windowWidth_in, int windowHeight_in, sf::RenderWindow* window);
};