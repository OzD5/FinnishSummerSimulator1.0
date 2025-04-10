#include "StartMenu.h"

void StartMenu::initWindow()
{
	this->videoMode.height = 1080;
	this->videoMode.width = 1080;

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
}

void StartMenu::initUI()
{

	//BACKGROUND
	if (!this->backGroundFile.loadFromFile("Data/Graphics/MainMenuBG.png"))
	{
		std::cout << "STARTMENU::INITUI::BACKGROUND NOT LOADED";
	}
	if (!this->uiBoxFile.loadFromFile("Data/Graphics/MainMenuBox.png"))
	{
		std::cout << "STARTMENU::INITUI::MENUBOX NOT LOADED";
	}
	if (!this->font.loadFromFile("Data/Fonts/UI_font.ttf"))
	{
		std::cout << "STARTMENU::INITUI::FONT NOT LOADED";
	}

	this->backGround.setTexture(this->backGroundFile);
	this->backGround.setPosition(0, 0);

	this->startButton.setTexture(this->uiBoxFile);
	this->startButton.setScale(sf::Vector2f(1.75f, 1.75f));
	this->startButton.setPosition(sf::Vector2f(325.f, 550.f));
	this->startButton.setColor(sf::Color(255, 255, 255, 200));

	this->difficultyButton.setTexture(this->uiBoxFile);
	this->difficultyButton.setScale(sf::Vector2f(1.75f, 1.75f));
	this->difficultyButton.setPosition(sf::Vector2f(325.f, 650.f));
	this->difficultyButton.setColor(sf::Color(255, 255, 255, 200));

	this->exitButton.setTexture(this->uiBoxFile);
	this->exitButton.setScale(sf::Vector2f(1.75f, 1.75f));
	this->exitButton.setPosition(sf::Vector2f(325.f, 750.f));
	this->exitButton.setColor(sf::Color(255, 255, 255, 200));

	//TEXT
	if (!this->font.loadFromFile("Data/Fonts/UI_font.ttf"))
	{
		std::cout << "STARTMENU::INITUI::FONT NOT LOADED";
	}

	this->startText.setFont(this->font);
	this->startText.setCharacterSize(45);
	this->startText.setFillColor(sf::Color::White);
	this->startText.setOutlineColor(sf::Color::Black);
	this->startText.setString("Begin journey");
	this->startText.setPosition(sf::Vector2f(350.f, this->startButton.getPosition().y+12.f));

	this->difficultyText.setFont(this->font);
	this->difficultyText.setCharacterSize(45);
	this->difficultyText.setFillColor(sf::Color::White);
	this->difficultyText.setOutlineColor(sf::Color::Black);
	this->difficultyText.setString("Easy");
	this->difficultyText.setPosition(sf::Vector2f(470.f, this->difficultyButton.getPosition().y+12.f));

	this->exitText.setFont(this->font);
	this->exitText.setCharacterSize(45);
	this->exitText.setFillColor(sf::Color::White);
	this->exitText.setOutlineColor(sf::Color::Black);
	this->exitText.setString("Exit game");
	this->exitText.setPosition(sf::Vector2f(420.f, this->exitButton.getPosition().y+12.f));
}

void StartMenu::initSound()
{
	if (!(this->mainMenuMusic).openFromFile("Data/Sounds/mainMenuOST.wav"))
	{
		std::cout << "STARTMENU::INITSOUND::MAINMENUOST.WAW NOT FOUND";
	}
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
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
			if (this->difficulty >= 4)
				this->difficulty = 1;
			else
				this->difficulty++;
			switch (difficulty)
			{
			case 1:
				this->difficultyText.setString("Easy");
				this->difficultyText.move(sf::Vector2f(100.f, 0));
				break;
			case 2:
				this->difficultyText.setString("Medium");
				this->difficultyText.move(sf::Vector2f(-10.f, 0));
				break;
			case 3:
				this->difficultyText.setString("Hard");
				this->difficultyText.move(sf::Vector2f(30.f, 0));
				break;
			case 4:
				this->difficultyText.setString("Suomalainen");
				this->difficultyText.move(sf::Vector2f(-120.f, 0));
			}
		}
		else if (this->exitButton.getGlobalBounds().contains(this->mousePosView))
		{
			this->endMenu = true;
		}
	}
	else this->mouseHold = false;
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
	target.draw(exitButton);
	target.draw(startText);
	target.draw(difficultyText);
	target.draw(exitText);
}

void StartMenu::render()
{
	this->startwindow->clear(sf::Color(0, 20, 255));

	this->startwindow->draw(this->backGround);

	this->renderButtons(*this->startwindow);

	this->startwindow->display();
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

StartMenu::StartMenu()
{
	this->initVariables();
	this->initWindow();
	this->initUI();
	this->initSound();
}

StartMenu::~StartMenu()
{
}
