#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


class PauseMenu
{
public:
	explicit PauseMenu(sf::RenderWindow* window, const int& windowWidth, const int& windowHeight);

	update();
	render();
private:
	sf::RenderWindow* window;

	//Graphics
	sf::Font font;
	sf::Text continueText;
	sf::Text exitText;
	sf::Sprite continueButton;
	sf::Sprite exitButton;

};