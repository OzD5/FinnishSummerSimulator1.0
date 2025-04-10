#include "Game.h"
#include "Save.h"
#include "Enemy.h"
#include <iostream>

//Private funktiot
void Game::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->highscore = Save::getHighscore();
	this->points = 0;
	this->enemySpawnTimerMax = 200.f;
	if (this->difficulty == 4)
	{
		this->enemySpawnTimer = 0.001f;
		this->enemySpawnTimerMax = 0.001f;
		this->maxEnemies = 1000000000;
		this->health = 1000000000;
	}
	else
	{
		this->enemySpawnTimer = this->enemySpawnTimerMax;
		this->maxEnemies = 7 * this->difficulty;
		this->health = 1000 - 100 * this->difficulty;
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
	this->speedY = 1.f*this->difficulty;
	this->mltplr = 1;
}

void Game::initWindow()
{
	this->videoMode.height = 1080;
	this->videoMode.width = 1920;

	this->window = new sf::RenderWindow(this->videoMode, "Mosquito Game", sf::Style::Titlebar | sf::Style::Close);
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
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(48);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setOutlineColor(sf::Color::Black);
	this->uiText.setOutlineThickness(0.5);
	this->uiText.setString("NONE");

	//Healthbar and Stamina Bar
	this->healthBar.setSize(sf::Vector2f(200.f, 20.f));
	this->healthBar.setPosition(sf::Vector2f(20.f, 200.f));
	this->healthBar.setFillColor(sf::Color::Green);

	this->staminaBar.setSize(sf::Vector2f(200.f, 20.f));
	this->staminaBar.setPosition(sf::Vector2f(20.f, 235.f));
	this->staminaBar.setFillColor(sf::Color::Blue);

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
		this->bloodBathObj.setScale(2.0f, 2.0f);
	}
	if (!(this->backGround).loadFromFile("Data/Graphics/GameBG.png"))
	{
		std::cout << "GAME::INITGRAPHICS::Background.jpg NOT FOUND";
	}
	else
	{
		this->backGroundObj.setTexture(backGround);
		this->backGroundObj.setScale(1.3f, 1.1f);
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
		musicFile = "Data/Sounds/mainOST4.ogg";
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
	this->hairyHand.setScale(sf::Vector2f(2.1f, 1.f));
	this->hairyHand.setPosition(-10.f, 700.f);
}

//Constructori/Destructori
Game::Game(short difficulty)
{
	this->difficulty = difficulty;
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

void Game::spawnEnemy()
{
	/*
		Spawn enemies with random color and random enemy
	*/
	int type = (rand() % 7) + 1;
	sf::Texture* mosquitoPic;
	if (rand() % 2 == 1)
		mosquitoPic = &mosquitoL;
	else mosquitoPic = &mosquitoR;
	//Init new enemy
	Enemy newEnemy(*mosquitoPic, sf::Vector2f(static_cast<float>(rand() % (static_cast<int>(this->window->getSize().x - 50.f)) + 50.f),
		0.f), type,(rand() % 100)*10);

	switch (type)
	{
	case 7:
		newEnemy.setSize(0.01f*sf::Vector2f(55.f, 30.f));
		break;
	case 6:
		newEnemy.setSize(0.01f * sf::Vector2f(60.f , 70.f ));
		break;
	case 5:
		newEnemy.setSize(0.01f * sf::Vector2f(65.f , 30.f ));
	case 4:
		newEnemy.setSize(0.01f * sf::Vector2f(85.f , 50.f ));
		break;
	case 3:
		newEnemy.setSize(0.01f * sf::Vector2f(45.f, 90.f ));
		break;
	case 2:
		newEnemy.setSize(0.01f * sf::Vector2f(70.f , 70.f ));
		break;
	case 1:
		newEnemy.setSize(0.01f * sf::Vector2f(75.f , 95.f));
		break;
	default:
		newEnemy.setSize(0.01f * sf::Vector2f(100.f , 90.f ));
		break;
	}
	this->enemies.push_back(newEnemy);

}
//Public Funktiot
void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
			//Ikkunan sulkeminen
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
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
	std::stringstream ss;
	ss << "Highscore: " << this->highscore << "\n"
		<< "Points: " << this->points << "\n"
		<< "Health: " << this->health;
	this->uiText.setString(ss.str());
	if (this->health < 2000) this->healthBar.setSize(sf::Vector2f(200.f * this->health / 1000, 20.f));
	else this->healthBar.setSize(sf::Vector2f(200.f, 20.f));
	//If you miss an enemy stamina bar gets smaller. It is changed in updateEnemies because its simpler.
	float curStamina = this->staminaBar.getSize().x;
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

void Game::moveEnemies()
{
	//Check if theres room for more enemies
	if (this->enemies.size() < static_cast<size_t>(this->maxEnemies))
	{
		//Check if it is time to spawn enemy, otherwise increase time
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			if (this->difficulty == 4)
			{
				for (int i = 0; i < 10; i++) this->spawnEnemy();
			}
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}
	// Moving and updating enemies
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		float enemyX = this->enemies[i].getPosition().x;
		float enemyY = this->enemies[i].getPosition().y;
		if (enemyY + this->enemies[i].getSize().y >= this->window->getSize().y- (25 + 25 * this->enemies[i].getVelocity()+this->enemies[i].getOffset()/13))
		{
			isTouching = true;
			float floatXdir = (rand() % 2 == 0) ? 0.1f : -0.1f;
			this->enemies[i].move(floatXdir,0.f);
			if (static_cast<int>(rand() % 10 == 0))
				this->health -= 1;
		}
		else
		{
			//
			float angle;
			float waveX;
			if (this->enemies[i].getVelocity() % 2 == 0)
			{
				angle = enemyY / 100.0f + this->enemies[i].getOffset();
				waveX = (sin(sin(angle)) * static_cast<float>(pow(cos(angle), 4)) + sin(2 * angle)) * this->speedX * 0.1f;
			}
			else
			{
				angle = enemyY / 110.0f + this->enemies[i].getOffset();
				waveX = (sin(angle) * static_cast<float>(pow(cos(angle), 3)) + sin(4 * angle)) * this->speedX * 0.1f;
			}
			this->enemies[i].move(waveX + (2 * (rand() % 2) - 1) * 0.2f, speedY);
		}
		if (this->enemies[i].getPosition().x + this->enemies[i].getSize().x >= this->window->getSize().x)
			this->enemies[i].move(-1.0f , 0);
		//this->enemies[i].move(static_cast<float>(1920) * float(-1.0) + this->enemies[i].getSize().x + float(1), 0);
		else if (this->enemies[i].getPosition().x < 0)
		{
			this->enemies[i].move(1.0f, 0);
		}

	}
	this->isTouching = false;
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
	/*this->mousePos = sf::Mouse::getPosition(*this->window);
	std::cout << "Mouse pos: "
		<< mousePos.x << " "
		<< mousePos.y << "\n";
	*/
	if (!this->endGame)
	{
		this->updateMousePos();

		this->updateUi();

		this->updateSpeed();

		this->moveEnemies();

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
	target.draw(this->uiText);
	target.draw(this->healthBar);
	target.draw(this->staminaBar);
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
