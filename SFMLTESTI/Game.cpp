#include "Game.h"
#include "Save.h"
#include "Enemy.h"
#include <iostream>

//Private funktiot
void Game::initVariables()
{
	this->window = nullptr;
	this->widthRatio = this->windowWidth / 1920.f;
	this->heightRatio = this->windowHeight / 1080.f;
	//Game logic
	this->endGame = false;
	this->highscore = Save::getHighscore();
	this->points = 0;
	this->enemySpawnTimerMax = 200.f;
	if (this->difficulty == 4)
	{
		this->enemySpawnTimer = 1.f;
		this->enemySpawnTimerMax = 1.f;
		this->maxEnemies = 1000;
		this->health = 10000000;
	}
	else
	{
		this->enemySpawnTimer = this->enemySpawnTimerMax;
		this->maxEnemies = 7 * this->difficulty;
		this->health = 1100 - 100 * this->difficulty;
	}
	/*
	this->enemySpawnTimerMax = 0.1f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10000000000 * this->difficulty;
	this->health = 10000000000;
	*/
	this->mouseHeld = false;
	this->isTouching = false;
	this->isMiss = false;
	this->isStaminaRegen = false;
	this->speedX = 5.f;
	this->speedY = 1.f * this->difficulty;
	this->mltplr = 1;
}

void Game::initWindow()
{
	this->videoMode.height = this->windowHeight;
	this->videoMode.width = this->windowWidth;

	this->window = new sf::RenderWindow(this->videoMode, "Finnish Summer Simulator", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(240);

}

void Game::initFonts()
{
	if (!this->font.loadFromFile("Data/Fonts/UI_font.ttf"))
	{
		std::cout << "GAME::INITFONTS::FONT NOT LOADED";
	}

}

void Game::initUi()
{
	//Scores etc. in uitext
	this->scoreText.setFont(this->font);
	this->scoreText.setCharacterSize(48);
	this->scoreText.setFillColor(sf::Color::White);
	this->scoreText.setOutlineColor(sf::Color::Black);
	this->scoreText.setOutlineThickness(0.5);
	this->scoreText.setString("NONE");
	this->scoreText.setScale(this->widthRatio, this->heightRatio);

	//Healthbar, Stamina Bar and their outlines

	initBar(healthBar, 20.f, 950.f, sf::Color::Green);
	initBar(staminaBar, 20.f, 900.f, sf::Color::Blue);

	initOutline(this->healthOutline, 20.f, 950.f);
	initOutline(this->staminaOutline, 20.f, 900.f);

	initText(scoreText, "NONE", 48, 5.f*this->widthRatio, 10.f*this->heightRatio);
	initText(this->healthText, "Health: ", 15,
		this->healthBar.getPosition().x + this->healthBar.getGlobalBounds().width + 20.f,
		this->healthBar.getPosition().y);
	initText(this->staminaText, "Stamina: ", 15,
		this->staminaBar.getPosition().x + this->staminaBar.getGlobalBounds().width + 20.f,
		this->staminaBar.getPosition().y);
}

void Game::initText(sf::Text& textObj, std::string text, int size, float x, float y) const
{
	textObj.setFont(this->font);
	textObj.setCharacterSize(size);
	textObj.setFillColor(sf::Color::White);
	textObj.setOutlineColor(sf::Color::Black);
	textObj.setOutlineThickness(0.5);
	textObj.setString(text);
	textObj.setPosition(x, y);
	textObj.setScale(this->widthRatio, this->heightRatio);

}

void Game::initBar(sf::RectangleShape& bar, float x, float y, sf::Color color) const
{
	bar.setSize(sf::Vector2f(200.f, 20.f));
	bar.setPosition(sf::Vector2f(this->widthRatio * x, this->heightRatio * y));
	bar.setFillColor(color);
	bar.setScale(this->widthRatio * 1.5f, this->heightRatio);
}

void Game::initOutline(sf::RectangleShape& outline, float x, float y) const
{
	outline.setSize(sf::Vector2f(200.f, 20.f));
	outline.setPosition(sf::Vector2f(this->widthRatio * x, this->heightRatio * y));
	outline.setFillColor(sf::Color(0, 0, 0, 0));
	outline.setOutlineColor(sf::Color::Black);
	outline.setOutlineThickness(-1.f);
	outline.setScale(this->widthRatio * 1.5f, this->heightRatio);
}

void Game::initGraphics()
{
	if (!(this->bloodBath).loadFromFile("Data/Graphics/BloodBath.png"))
	{
		std::cout << "GAME::INITGRAPHICS::BLOODBATH.PNG NOT FOUND";
	}
	else
	{
		this->bloodBathObj.setTexture(bloodBath);
		this->bloodBathObj.setScale(2.0f * this->widthRatio, 2.0f * this->heightRatio);
	}
	if (!(this->backGround).loadFromFile("Data/Graphics/GameBG.png"))
	{
		std::cout << "GAME::INITGRAPHICS::Background.jpg NOT FOUND";
	}
	else
	{
		this->backGroundObj.setTexture(backGround);
		this->backGroundObj.setScale(1.3f * this->widthRatio, 1.1f * this->heightRatio);
		this->backGroundObj.setPosition(0, 0);
	}
	//Jos haluut brainrot V:n niin enemy1 ja enemy.png
	if (!this->mosquitoL.loadFromFile("Data/Graphics/mosquitoL.png"))
	{
		std::cout << "ENEMY::INITGRAPHICS::MOSQUITOL.PNG NOT FOUND";
	}
	if (!this->mosquitoR.loadFromFile("Data/Graphics/mosquitoR.png"))
	{
		std::cout << "ENEMY::INITGRAPHICS::MOSQUITOR.PNG NOT FOUND";
	}
}
void Game::initSounds()
{
	//Initting music
	std::string musicFile;
	switch (this->difficulty)
	{
	case 1:
		musicFile = "Data/Sounds/mainOST1.ogg";
		break;
	case 2:
		musicFile = "Data/Sounds/mainOST2.wav";
		break;
	case 3:
		musicFile = "Data/Sounds/mainOST2.2.wav";
		break;
	case 4:
		musicFile = "Data/Sounds/mainOST2.wav";
		break;
	default:
		musicFile = "Data/Sounds/mainOST2.wav";
		break;
	}

	if (!(this->musicOST).openFromFile(musicFile))
	{
		std::cout << "GAME::INITMUSIC::MAINOST1.OGG NOT FOUND";
	}
	this->musicOST.setLoop(true);
	this->musicOST.setVolume(20);
	this->musicOST.play();

	//Initting sound FX
	if (!(this->hittingHandFile).loadFromFile("Data/Sounds/HittingHand.ogg"))
	{
		std::cout << "GAME::INITMUSIC::HITTINGHAND.OGG NOT FOUND";
	}
	this->hittingHandSound.setBuffer(this->hittingHandFile);

	if (!(this->swingingAirFile).loadFromFile("Data/Sounds/SwingingAir.ogg"))
	{
		std::cout << "GAME::INITMUSIC::SWINGINGAIR.OGG NOT FOUND";
	}
	this->swingingAirSound.setBuffer(this->swingingAirFile);

	if (!(this->hittingInsectFile).loadFromFile("Data/Sounds/hittingInsect.ogg"))
	{
		std::cout << "GAME::INITMUSIC::HITTINGINSECT.OGG NOT FOUND";
	}
	this->hittingInsectSound.setBuffer(this->hittingInsectFile);
	this->hittingInsectSound.setVolume(50);


}
void Game::initHand()
{
	//Hand object
	if (!this->handTexture.loadFromFile("Data/Graphics/HairyArm.png"))
	{
		std::cout << "ENEMY::INITGRAPHICS::HAIRYARM.PNG NOT FOUND";
	}
	this->hairyHand.setTexture(this->handTexture);
	this->hairyHand.setScale(sf::Vector2f(2.1f * this->widthRatio, 0.9f * this->heightRatio));
	this->hairyHand.setPosition(-10.f, 0.6f * this->windowHeight);
}

//Constructori/Destructori
Game::Game(short difficulty, unsigned windowWidth, unsigned windowHeight)
{
	this->difficulty = difficulty;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->initVariables();
	this->initWindow();
	this->initHand();
	this->initFonts();
	this->initUi();
	this->initGraphics();
	this->initSounds();
}

Game::~Game()
{
	Save::updateHighscore(this->points);
	delete this->window;
}

//Always check if game is running
const bool Game::running() const
{
	//Check that window isn't a null pointer
	if (this->window) {
		return this->window->isOpen();
	}
	return false;
}

const bool Game::getEndGame() const
{
	return this->endGame;
}


//C
void Game::spawnEnemy()
{
	/*
		Spawn enemies with random color, random type, and random velocity
	*/
	int type = (rand() % 7) + 1;
	//Look direction of mosquito.
	sf::Texture* mosquitoPic = (rand() % 2 == 1) ? mosquitoPic = &mosquitoL : &mosquitoR;
	//Init new enemy
	Enemy newEnemy(*mosquitoPic, sf::Vector2f(static_cast<float>(rand() % (static_cast<int>(this->window->getSize().x - 50.f)) + 50.f),
		0.f), type, (rand() % 100) * 10);

	std::map<int, sf::Vector2f> sizeMap = {
	  {7, sf::Vector2f(55.f, 30.f)},
	  {6, sf::Vector2f(60.f, 70.f)},
	  {5, sf::Vector2f(65.f, 30.f)},
	  {4, sf::Vector2f(85.f, 50.f)},
	  {3, sf::Vector2f(45.f, 90.f)},
	  {2, sf::Vector2f(70.f, 70.f)},
	  {1, sf::Vector2f(75.f, 95.f)}
	};
	newEnemy.setSize(0.01f * sizeMap[type]);

	this->enemies.push_back(newEnemy);

}
//Public Funktiot
void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape) this->window->close();
			break;
		}
	}
}

void Game::updateMousePos()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateUi()
{
	if (this->points > this->highscore) this->highscore = this->points;
	std::stringstream scoreString;
	scoreString << "Highscore: " << this->highscore << "\n"
		<< "Points: " << this->points << "\n";
	this->scoreText.setString(scoreString.str());

	//Health
	if (this->health < 2000) this->healthBar.setSize(sf::Vector2f(200.f * this->health / 1000, 20.f));
	else this->healthBar.setSize(sf::Vector2f(200.f, 20.f));
	
	std::stringstream healthString;
	healthString << "Health: " << this->health;
	this->healthText.setString(healthString.str());



	//Stamina
	float curStamina = this->staminaBar.getSize().x;
	std::stringstream staminaString;
	staminaString << "Stamina: " << curStamina;
	this->staminaText.setString(staminaString.str());


	//If you miss an enemy stamina bar gets smaller. It is changed in updateEnemies because its simpler.

	if (curStamina < 199.f && !isStaminaRegen)
	{
		this->regenClock.restart();
		isStaminaRegen = true;
	}
	if (this->regenClock.getElapsedTime().asSeconds() >= 1 && curStamina < 199.f)
	{
		this->regenClock.restart();
		this->staminaBar.setSize(sf::Vector2f(curStamina + 25.f, 20.f));
	}
}

void Game::updateSpeed()
{
	if (this->points > 25 * mltplr)
	{
		if (this->speedY < 2)
			this->speedY *= 1.01f;
		else if (this->speedY < 2.5) this->speedY += 0.02f;
		this->mltplr++;
	}
}

void Game::deleteEnemy()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!this->mouseHeld)
		{
			this->mouseHeld = true;
			bool deleted = false;
			bool hitSkin = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				//Check if you hit enemy and that there's enough stamina
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView) &&
					(staminaBar.getSize().x > 1.f ||
						(this->enemies[i].getPosition().y + this->enemies[i].getSize().y >= this->window->getSize().y - (25 + 30 * this->enemies[i].getVelocity()))))
				{
					//Getting points based on difficulty of enemy
					if (this->enemies[i].getVelocity() > 5)
						this->points += 20;
					else if (this->enemies[i].getVelocity() > 3)
						this->points += 15;
					else if (this->enemies[i].getVelocity() > 1)
						this->points += 10;
					else this->points += 5;

					//delete enemy and break loop so that we dont iterate over empty vector lmao
					if (this->enemies[i].getPosition().y + this->enemies[i].getSize().y >= this->window->getSize().y - (25 + 25 * this->enemies[i].getVelocity() + this->enemies[i].getOffset() / 13)) hitSkin = true;
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
			if (!deleted || hitSkin)
			{
				//Staminabar updates if we have stamina and health decreases when we miss
				if (staminaBar.getSize().x > 0 && hitSkin)
				{
					this->health -= 10;
					this->isMiss = true;
					this->bloodClock.restart();
					//this->staminaBar.setSize(sf::Vector2f(staminaBar.getSize().x - 25.f, 20.f));

					this->hittingHandSound.play();
				}
				else if (staminaBar.getSize().x > 0 && !hitSkin)
				{
					this->isStaminaRegen = false;
					this->isMiss = true;
					this->staminaBar.setSize(sf::Vector2f(staminaBar.getSize().x - 25.f, 20.f));

					if (this->hairyHand.getGlobalBounds().contains(this->mousePosView))
						this->hittingHandSound.play();
					else
						this->swingingAirSound.play();
				}

			}
			else
				this->hittingInsectSound.play();
		}
	}
	else this->mouseHeld = false;
}


void Game::updateEnemies()
{
	// Check if there's room for more enemies and spawn if necessary
	if (this->enemies.size() < static_cast<size_t>(this->maxEnemies))
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 1.f;
		}
	}

	// Moving and updating enemies
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		updateEnemyPosition(this->enemies[i]);
		checkEnemyBounds(this->enemies[i]);
	}

	this->isTouching = false;
}

void Game::updateEnemyPosition(Enemy& enemy)
{
	float enemyX = enemy.getPosition().x;
	float enemyY = enemy.getPosition().y;

	if (enemyY + enemy.getSize().y >= this->window->getSize().y - this->heightRatio * (60 + 30 * enemy.getVelocity() + enemy.getOffset() / 13))
	{
		this->isTouching = true;
		if (static_cast<int>(rand() % 10) == 0)
		{
			this->health -= 1;
		}
	}
	else
	{
		float waveX = calculateWaveX(enemyY, enemy.getVelocity(), enemy.getOffset());
		enemy.move(waveX + (2 * (rand() % 2) - 1) * 0.2f, this->speedY);
	}
}

float Game::calculateWaveX(float enemyY, int velocity, int offset) const
{
	float angle;
	float randomness = static_cast<float>(rand() % 100) / 100;
	if (velocity % 2 == 0)
	{
		angle = (enemyY / 100.0f + offset) * 0.1f;
		return (sin(sin(angle) * static_cast<float>(pow(cos(angle), 4)) + sin(0.5f * angle) * this->speedX * 0.1f) * sin(angle) * randomness) * this->widthRatio;
	}
	else
	{
		angle = (enemyY / 110.0f + offset) * 0.1f;
		return (sin(angle * static_cast<float>(pow(cos(angle), 3)) + sin(angle) * this->speedX) * cos(angle) * randomness) * this->widthRatio;
	}
}

void Game::checkEnemyBounds(Enemy& enemy)
{
	if (enemy.getPosition().x + enemy.getSize().x >= this->window->getSize().x)
	{
		enemy.move(-1.0f, 0);
	}
	else if (enemy.getPosition().x < 0)
	{
		enemy.move(1.0f, 0);
	}
}


void Game::moveHand()
{
	if (rand() % 10 == 0)
	{
		float shakeX = (rand() % 3 - 1) * 0.25f;
		float shakeY = (rand() % 3 - 1) * 0.1f;
		this->hairyHand.move(sf::Vector2f(shakeX, shakeY));
	}
}

void Game::update()
{
	this->pollEvents();

	//Update mouse pos
	if (!this->endGame)
	{
		this->updateMousePos();

		this->updateUi();

		this->updateSpeed();

		this->updateEnemies();

		this->moveHand();

		this->deleteEnemy();

	}
	std::cout << this->enemies.size() << "\n";
	//health check
	if (this->health <= 0)
	{
		this->endGame = true;
	}
}

void Game::renderBlood(sf::RenderTarget& target)
{
	float elapsedTime = this->bloodClock.getElapsedTime().asSeconds();
	if (elapsedTime <= 1)
	{
		uint8_t alpha = static_cast<uint8_t>(255 * (1.0f - (elapsedTime / 1)));
		this->bloodBathObj.setColor(sf::Color(255, 255, 255, alpha));
		target.draw(bloodBathObj);
	}
	else
	{
		this->isMiss = false;
		bloodBathObj.setColor(sf::Color(255, 255, 255, 255));
	}
}

void Game::renderUi(sf::RenderTarget& target)
{
	target.draw(this->scoreText);
	target.draw(this->healthBar);
	target.draw(this->staminaBar);
	target.draw(this->healthOutline);
	target.draw(this->staminaOutline);
	target.draw(this->healthText);
	target.draw(this->staminaText);
}


void Game::renderRects(sf::RenderTarget& target)
{
	target.draw(this->hairyHand);
	//render the enemies
	for (auto& enemi : this->enemies)
	{
		target.draw(enemi.getEnemySprite());
	}
}

void Game::render()
{
	/*
		Renders game objects. First clear the last frame, then render obejcts, then display frame
	*/
	this->window->clear(sf::Color(135, 206, 250));
	//Render Background first
	this->window->draw(this->backGroundObj);
	//Game draw here
	this->renderRects(*this->window);
	if (isMiss)
		this->renderBlood(*this->window);
	this->renderUi(*this->window);

	this->window->display();
}
