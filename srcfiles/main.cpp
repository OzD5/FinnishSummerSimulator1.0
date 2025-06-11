#include <iostream>
#include "StartMenu.h"
#include "Game.h"
#include "GameManager.h"

int main()
{
	//1,2,3,4 difficulty = easy,medium,hard, suomalainen
	unsigned windowWidth = 1920;
	unsigned windowHeight = 1080;
	sf::RenderWindow* window = new sf::RenderWindow (sf::VideoMode(1920, 1080), "Finnish Summer Simulator",sf::Style::Titlebar | sf::Style::Default);
	//init rand gen
	std::srand(static_cast<unsigned>(time(NULL)));

	// Starting game management
	GameManager::GameManager(windowWidth, windowHeight, window);


	return 0;
}