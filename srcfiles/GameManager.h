#pragma once
#include "Game.h"
#include "StartMenu.h"

class GameManager
{
private:
	int windowWidth;
	int windowHeight;
	short difficulty;
	void changeResoluton(short resolution);
	void runStartMenu(int windowWidth, int windowHeight, sf::RenderWindow* window);
	void runGame(short difficulty, int windowWidth, int windowHeight, sf::RenderWindow* window);
public:
	explicit GameManager(int windowWidth_in, int windowHeight_in, sf::RenderWindow* window);
};