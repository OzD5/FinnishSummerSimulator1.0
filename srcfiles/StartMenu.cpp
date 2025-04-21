#include "StartMenu.h"

void StartMenu::initWindow()
{
	this->videoMode.height = this->windowHeight;
	this->videoMode.width = this->windowWidth;
	this->windowRatio = this->windowWidth / 1920.f;

	//Dynamically creating new window.
	this->startwindow = std::make_unique<sf::RenderWindow>(this->videoMode, "Finnish Summer Simulator", sf::Style::Titlebar | sf::Style::Close);
	this->startwindow->setFramerateLimit(60);
}

void StartMenu::initVariables()
{
	this->startwindow = nullptr;

	this->difficulty = 1;
	this->mouseHold = false;
	this->endMenu = false;
	this->startGame = false;
	this->resolution = 2;
}

void StartMenu::initUI()
{

	//BACKGROUND
	if (!loadRecources())
	{
		std::cerr << "STARTMENU::INITUNI::RECOURCES NOT LOADED" << std::endl;
	}

	this->backGround.setTexture(this->backGroundFile);
	this->backGround.setPosition(0, 0);
	this->backGround.setScale(sf::Vector2f(windowRatio, windowRatio));

	float centerWidth = static_cast<float>(this->windowWidth / 2);
	float centerHeight = static_cast<float>(this->windowHeight / 2);
	setupButton(startButton, centerWidth, centerHeight);
	setupButton(difficultyButton, centerWidth, centerHeight + 100.f*windowRatio);
	setupButton(resolutionButton, centerWidth, centerHeight + 200.f*windowRatio);
	setupButton(exitButton, centerWidth, centerHeight + 300.f*windowRatio);

	setupText(this->startText, "Begin journey", startButton.getPosition().x, startButton.getPosition().y);
	setupText(this->difficultyText, "Easy", difficultyButton.getPosition().x, difficultyButton.getPosition().y);
	setupText(this->resolutionText, "1080p", resolutionButton.getPosition().x, resolutionButton.getPosition().y);
	setupText(this->exitText, "Exit game", exitButton.getPosition().x, exitButton.getPosition().y);


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
	buttonText.setCharacterSize(45*windowRatio);
	buttonText.setFillColor(sf::Color::White);
	buttonText.setOutlineColor(sf::Color::Black);
	buttonText.setString(text);
	buttonText.setPosition(sf::Vector2f(x + this->exitButton.getGlobalBounds().width/2 - buttonText.getGlobalBounds().width/2, y + 12.f*windowRatio));
}


void StartMenu::initSound()
{
	if (!(this->mainMenuMusic).openFromFile("Data/Sounds/mainMenuOST.wav"))
	{
		std::cout << "STARTMENU::INITSOUND::MAINMENUOST.WAW NOT FOUND";
	}
	this->mainMenuMusic.setLoop(true);
	this->mainMenuMusic.setVolume(14);
	this->mainMenuMusic.play();
}

void StartMenu::pollEvents()
{
	while (this->startwindow->pollEvent(this->eve))
	{
		if (this->eve.type == sf::Event::Closed)
		{
			this->startwindow->close();
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


void StartMenu::checkPress()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->mouseHold = false;
		return;
	}
	if (this->mouseHold)
		return;
	this->mouseHold = true;
	if (this->startButton.getGlobalBounds().contains(this->mousePosView))
	{
		this->endMenu = true;
		this->startGame = true;
	}
	else if (this->difficultyButton.getGlobalBounds().contains(this->mousePosView))
	{
		updateDifficulty();
	}
	else if (this->resolutionButton.getGlobalBounds().contains(this->mousePosView))
	{
		updateResolution();
	}
	else if (this->exitButton.getGlobalBounds().contains(this->mousePosView))
	{
		this->endMenu = true;
	}
}

void StartMenu::updateDifficulty()
{
	if (this->difficulty >= 4)
		this->difficulty = 1;
	else
		this->difficulty++;
	switch (difficulty)
	{
	case 1:
		this->difficultyText.setString("Easy");
		break;
	case 2:
		this->difficultyText.setString("Medium");
		break;
	case 3:
		this->difficultyText.setString("Hard");
		break;
	case 4:
		this->difficultyText.setString("Suomalainen");
		break;
	}
	this->difficultyText.setPosition(sf::Vector2f(
		difficultyButton.getPosition().x + this->exitButton.getGlobalBounds().width / 2 - difficultyText.getGlobalBounds().width / 2
		, difficultyButton.getPosition().y + 12.f));
}

void StartMenu::updateResolution()
{
	if (this->resolution >= 3)
		this->resolution = 1;
	else
		this->resolution++;
	switch (resolution)
	{
	case 1:
		this->resolutionText.setString("720p");
		break;
	case 2:
		this->resolutionText.setString("1080p");
		break;
	case 3:
		this->resolutionText.setString("1440p");
		break;
	default:
		break;
	}
	this->resolutionText.setPosition(sf::Vector2f(
		resolutionButton.getPosition().x + this->resolutionButton.getGlobalBounds().width / 2 - resolutionText.getGlobalBounds().width / 2
		, resolutionButton.getPosition().y + 12.f));
}

void StartMenu::updateMousePos()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->startwindow);
	this->mousePosView = this->startwindow->mapPixelToCoords(this->mousePosWindow);
}

void StartMenu::update()
{
	if (!this->endMenu)
	{
		this->pollEvents();
		this->updateMousePos();
		this->checkPress();
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
	this->startwindow->clear(sf::Color(0, 20, 255));

	this->startwindow->draw(this->backGround);

	this->renderButtons(*this->startwindow);

	this->startwindow->display();
}

short StartMenu::getResolution() const
{
	return this->resolution;
}

short StartMenu::getDifficulty() const
{
	return this->difficulty;
}

bool StartMenu::getEndMenu() const
{
	return this->endMenu;
}

bool StartMenu::getStartGame() const
{
	return this->startGame;
}

const bool StartMenu::running() const
{
	if (this->startwindow) {
		return this->startwindow->isOpen();
	}
	return false;
}

StartMenu::StartMenu(unsigned width, unsigned height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	this->initVariables();
	this->initWindow();
	this->initUI();
	this->initSound();
}

StartMenu::~StartMenu()
{
}
