#include <iostream>
#include "StartMenu.hpp"
#include "Game.hpp"
#include "GameManager.hpp"
#include "prints.cpp"
int main()
{
	sf::VideoMode userScreen = sf::VideoMode::getDesktopMode();
	unsigned windowWidth = userScreen.width;
	unsigned windowHeight = userScreen.height;

	sf::RenderWindow* window = new sf::RenderWindow (sf::VideoMode(windowWidth, windowHeight), "Finnish Summer Simulator",sf::Style::Titlebar | sf::Style::Default);
	
	std::srand(static_cast<unsigned>(time(NULL)));

	GameManager gameManager = GameManager(windowWidth, windowHeight, window);
	gameManager.run();

	return 0;
}