#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class StartMenu
{
private:
	std::unique_ptr<sf::RenderWindow> startwindow;
	sf::Event eve;
	sf::VideoMode videoMode;

	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//UI/GRAPHICS
	sf::Font font;
	sf::Text startText;
	sf::Text difficultyText;
	sf::Text resolutionText;
	sf::Text exitText;
	

	sf::Texture backGroundFile;
	sf::Texture uiBoxFile;

	sf::Sprite backGround;
	sf::Sprite startButton;
	sf::Sprite difficultyButton;
	sf::Sprite resolutionButton;
	sf::Sprite exitButton;

	unsigned windowHeight;
	unsigned windowWidth;
	float windowRatio;
	bool endMenu;
	bool mouseHold;
	bool startGame;
	short difficulty;
	short resolution;
	//music
	sf::Music mainMenuMusic;

	void checkPress();
	void updateDifficulty();
	void updateResolution();
	void updateMousePos();
	void initWindow();
	void initVariables();
	void initUI();
	const bool loadRecources();
	void setupButton(sf::Sprite& button, float x, float y) const;
	void setupText(sf::Text& buttonText, const std::string& text, float x, float y) const;
	void initSound();
	void pollEvents();
	void renderButtons(sf::RenderTarget& target);
public:
	void update();
	void render();

	short getResolution() const;
	short getDifficulty() const;
	bool getEndMenu() const;
	bool getStartGame() const;
	const bool running() const;

	StartMenu(unsigned width, unsigned height);
	virtual ~StartMenu();
};