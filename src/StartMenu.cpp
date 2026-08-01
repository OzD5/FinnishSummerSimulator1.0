#include "StartMenu.hpp"


void StartMenu::update()
{
	if (!endMenu)
	{
		pollEvents();
		updateMousePos();
		checkPress();
	}
}

void StartMenu::renderButtons(sf::RenderTarget& target)
{
	target.draw(startButton);
	target.draw(difficultyButton);
	target.draw(resolutionButton);
	target.draw(exitButton);

	target.draw(startText);
	target.draw(difficultyText);
	target.draw(resolutionText);
	target.draw(exitText);
	
}

void StartMenu::render()
{
	startwindow->clear(sf::Color(0, 20, 255));

	startwindow->draw(backGround);

	renderButtons(*startwindow);

	startwindow->display();
}

void StartMenu::updateMousePos()
{
	mousePosWindow = sf::Mouse::getPosition(*startwindow);
	mousePosView = startwindow->mapPixelToCoords(mousePosWindow);
}

void StartMenu::silenceMusic()
{
	mainMenuMusic.pause();
}

short StartMenu::getResolution() const
{
	return resolution;
}

short StartMenu::getDifficulty() const
{
	return difficulty;
}

bool StartMenu::getEndMenu() const
{
	return endMenu;
}

bool StartMenu::getStartGame() const
{
	return startGame;
}

const bool StartMenu::running() const
{
	if (startwindow) {
		return startwindow->isOpen();
	}
	return false;
}

StartMenu::StartMenu(unsigned width, unsigned height, sf::RenderWindow* window)
{
	windowWidth = width;
	windowHeight = height;
	initVariables();
	initWindow(window);
	initUI();
	initSound();
}

StartMenu::~StartMenu()
{
}

void StartMenu::initWindow(sf::RenderWindow* window)
{
	videoMode.height = windowHeight;
	videoMode.width = windowWidth;
	windowRatio = windowWidth / 1920.f;

	//Dynamically creating new window.
	startwindow = window;
	startwindow->setFramerateLimit(60);
}

void StartMenu::initVariables()
{
	startwindow = nullptr;

	difficulty = 1;
	mouseHold = false;
	endMenu = false;
	startGame = false;
	resolution = 2;
}

void StartMenu::initUI()
{

	//BACKGROUND
	if (!loadRecources())
	{
		std::cerr << "STARTMENU::INITUNI::RECOURCES NOT LOADED" << std::endl;
	}

	backGround.setTexture(backGroundFile);
	backGround.setPosition(0, 0);
	backGround.setScale(sf::Vector2f(windowRatio, windowRatio));

	float centerWidth = static_cast<float>(windowWidth / 2);
	float centerHeight = static_cast<float>(windowHeight / 2);

	setupButton(startButton, centerWidth, centerHeight);
	setupButton(difficultyButton, centerWidth, centerHeight + 100.f*windowRatio);
	setupButton(resolutionButton, centerWidth, centerHeight + 200.f*windowRatio);
	setupButton(exitButton, centerWidth, centerHeight + 300.f*windowRatio);

	//TODO RESOLUTION CHANGE NOT WORKING PROPERLY
	setupText(startText, "Begin journey", startButton.getPosition().x, startButton.getPosition().y);
	setupText(difficultyText, "Level 1", difficultyButton.getPosition().x, difficultyButton.getPosition().y);
	setupText(resolutionText, "1080p", resolutionButton.getPosition().x, resolutionButton.getPosition().y);
	setupText(exitText, "Exit game", exitButton.getPosition().x, exitButton.getPosition().y);


}

const bool StartMenu::loadRecources()
{
	return backGroundFile.loadFromFile("Data/Graphics/MainMenuBG.jpg")
		&& uiBoxFile.loadFromFile("Data/Graphics/MainMenuBox.png")
		&& font.loadFromFile("Data/Fonts/UI_font.ttf");
}
void StartMenu::setupButton(sf::Sprite& button, float x, float y) const
{
	button.setTexture(uiBoxFile);
	button.setScale(sf::Vector2f(1.65f*windowRatio, 1.65f * windowRatio));
	button.setPosition(sf::Vector2f(x - button.getGlobalBounds().width / 2, y));
	button.setColor(sf::Color(255, 255, 255, 200));
}

void StartMenu::setupText(sf::Text& buttonText, const std::string& text, float x, float y) const
{
	buttonText.setFont(font);
	buttonText.setCharacterSize(45*static_cast<unsigned>(windowRatio));
	buttonText.setFillColor(sf::Color::White);
	buttonText.setOutlineColor(sf::Color::Black);
	buttonText.setString(text);
	buttonText.setPosition(
		sf::Vector2f(x + exitButton.getGlobalBounds().width/2 
		- buttonText.getGlobalBounds().width/2, y + 12.f*windowRatio)
	);
}


void StartMenu::initSound()
{
	if (!(mainMenuMusic).openFromFile("Data/Sounds/mainMenuOST.wav"))
	{
		std::cout << "STARTMENU::INITSOUND::MAINMENUOST.WAW NOT FOUND";
	}
	mainMenuMusic.setLoop(true);
	mainMenuMusic.setVolume(14);
	mainMenuMusic.play();
}

void StartMenu::pollEvents()
{
	while (startwindow->pollEvent(eve))
	{
		if (eve.type == sf::Event::Closed)
		{
			startwindow->close();
			break;
		}
		/*else if (this->eve.type == sf::Event::MouseButtonPressed)
		{
			if (this->eve.mouseButton.button == sf::Mouse::Left)
			{
				this->leftClickPress = true;
			}
		}*/
	}
}

//Checks if you are pressing any button.
void StartMenu::checkPress()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		mouseHold = false;
		return;
	}
	if (mouseHold)
		return;
	mouseHold = true;
	if (startButton.getGlobalBounds().contains(mousePosView))
	{
		endMenu = true;
		startGame = true;
	}
	else if (difficultyButton.getGlobalBounds().contains(mousePosView))
	{
		updateDifficulty();
	}
	else if (resolutionButton.getGlobalBounds().contains(mousePosView))
	{
		updateResolution();
	}
	else if (exitButton.getGlobalBounds().contains(mousePosView))
	{
		endMenu = true;
	}
}

void StartMenu::updateDifficulty()
{
	if (difficulty >= 4) {
		difficulty = 1;
	}
	else {
		difficulty++;
	}
	switch (difficulty)
	{
	case 1:
		difficultyText.setString("Level 1");
		break;
	case 2:
		difficultyText.setString("Level 2");
		break;
	case 3:
		difficultyText.setString("Level 3");
		break;
	case 4:
		difficultyText.setString("Level 4");
		break;
	}
	//Centering the text
	difficultyText.setPosition(sf::Vector2f(
		difficultyButton.getPosition().x + exitButton.getGlobalBounds().width / 2 - difficultyText.getGlobalBounds().width / 2
		, difficultyButton.getPosition().y + 12.f));
}

void StartMenu::updateResolution()
{
	if (resolution >= 3) {
		resolution = 1;
	}
	else {
		resolution++;
	}
	switch (resolution)
	{
	case 1:
		resolutionText.setString("720p");
		break;
	case 2:
		resolutionText.setString("1080p");
		break;
	case 3:
		resolutionText.setString("1440p");
		break;
	default:
		break;
	}
	//Centering the text
	resolutionText.setPosition(sf::Vector2f(
		resolutionButton.getPosition().x + resolutionButton.getGlobalBounds().width / 2 - resolutionText.getGlobalBounds().width / 2
		, resolutionButton.getPosition().y + 12.f));
}