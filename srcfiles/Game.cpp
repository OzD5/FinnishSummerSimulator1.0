#include "Game.h"
#include "GameInitializer.h"
#include "Save.h"
#include "Enemy.h"
#include <iostream>


void Game::spawnEnemy()
{
	// Spawn enemies with random type, and random velocity
	int type = (rand() % 7) + 1;

	//Half of mosquitos look right and half look left
	sf::Texture* mosquitoPic = (rand() % 2 == 1) ? mosquitoPic = &mosquitoL : &mosquitoR;
	//Creating new enemy
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
	newEnemy.setSize(sf::Vector2f(0.003f * this->heightRatio * sizeMap[type].x, 0.003f * this->heightRatio * sizeMap[type].y));

	this->enemies.push_back(newEnemy);

}

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
	//UI scores
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


	//Stamina bar logic

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
	//If not enough points, no need to update
	if (this->points <= 25 * mltplr) return;

	if (this->speedY < 2)
		this->speedY *= 1.01f;
	else if (this->speedY < 2.5)
		this->speedY += 0.02f;
	this->mltplr++;
}

void Game::deleteEnemy()
{
	//If player hasn't pressed a button we just return and change the mouseheld var to false. 
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->mouseHeld = false;
		return;
	}
	//If mouse is held, we return
	if (this->mouseHeld)
	{
		return;
	}

	this->mouseHeld = true;
	bool deleted = false;
	bool hitSkin = false;
	for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
	{
		//Check if you hit enemy and that there's enough stamina
		bool enemyOnHand = mosquitoOnHand(enemies[i]);

		//If we havent hit the enemy we continue. Also if we hit but we dont have stamina and the enemy not in hand we continue
		if (!this->enemies[i].getGlobalBounds().contains(this->mousePosView) ||
			!(staminaBar.getSize().x > 1.f || enemyOnHand))
			continue;

		//Getting points based on difficulty of enemy. If enemy is already on hand we give 1 point only
		int scoreAmount = this->enemies[i].getVelocity();
		if (!enemyOnHand)
		{
			this->points += static_cast<int>(2.5 * scoreAmount) + 5;
		}
		else
		{
			this->points += 1;
			hitSkin = true;
		}
		//delete enemy and break loop so that we dont iterate more because we found our target and because our vector changes.
		deleted = true;
		this->enemies.erase(this->enemies.begin() + i);
		break;
	}
	// If we hit enemy in air
	if (deleted && !hitSkin)
	{
		this->hittingInsectSound.play();
		return;
	}
	//If we have stamina and we hit mosquito on skin
	if (staminaBar.getSize().x > 0 && hitSkin)
	{
		this->health -= 10;
		this->isMiss = true;
		this->bloodClock.restart();

		this->hittingHandSound.play();
	} //If we have stamina and we didnt hit mosquito
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

const bool Game::mosquitoOnHand(const Enemy& enemy) const
{
	return enemy.getPosition().y + enemy.getSize().y >= this->window->getSize().y - this->heightRatio * (60 + 30 * enemy.getVelocity() + enemy.getOffset() / 13);
}

void Game::updateEnemies()
{
	// Check if there's room for more enemies and spawn if necessary
	if (this->enemies.size() < static_cast<size_t>(this->maxEnemies))
	{
		if (this->enemySpawnTimer < this->enemySpawnTimerMax)
		{
			this->enemySpawnTimer += 1.f;
		}
		else
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
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

	if (!mosquitoOnHand(enemy))
	{
		//If enemy not on hand yet we calculate the trajectory and move it
		float waveX = calculateWaveX(enemyY, enemy.getVelocity(), enemy.getOffset());
		enemy.move(waveX + (2 * (rand() % 2) - 1) * 0.2f, this->speedY);
		return;
	}
	//If enemy already on hand we take health away
	this->isTouching = true;
	if (static_cast<int>(rand() % 10) == 0)
	{
		this->health -= 1;
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

//Constructor/Destructor
Game::Game(short difficultyIN, unsigned windowWidthIN, unsigned windowHeightIN)
	: difficulty(difficultyIN)
	, windowWidth(windowWidthIN)
	, windowHeight(windowHeightIN)
	, heightRatio(0.0f)
	, widthRatio(0.0f)
	, endGame(false)
	, highscore(0)
	, points(0)
	, enemySpawnTimer(0.0f)
	, enemySpawnTimerMax(0.0f)
	, maxEnemies(0)
	, health(0)
	, mouseHeld(false)
	, isTouching(false)
	, isMiss(false)
	, isStaminaRegen(false)
	, speedX(0.0f)
	, speedY(0.0f)
	, mltplr(0)
	, ev()
{
	//Initting the values of Game variables in Gameinitializer class
	GameInitializer::init(*this);
}

Game::~Game()
{
	Save::updateHighscore(this->points);
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
	//Health check
	if (this->health <= 0)
	{
		this->endGame = true;
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
