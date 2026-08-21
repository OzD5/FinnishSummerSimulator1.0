#include <cmath>
#include <iostream>
#include <random>
#include <format>
#include <algorithm>

#include "Game.hpp"
#include "GameInitializer.hpp"
#include "Save.hpp"
#include "Enemy.hpp"
#include "prints.cpp"

Game::Game(short difficultyIN, unsigned windowWidthIN, unsigned windowHeightIN, sf::RenderWindow* window)
	: difficulty(difficultyIN)
	, windowWidth(windowWidthIN)
	, windowHeight(windowHeightIN)
	, window(window)
{
	GameInitializer::init(*this, window);
}

Game::~Game()
{
	Save::updateHighscore(points, saveFilePath);
}

bool Game::running() const
{
	return window && window->isOpen();
}

bool Game::getEndGame() const
{
	return endGame;
}

bool Game::getPauseMenu() const
{
	return toPauseMenu;
}


void Game::silenceMusic()
{
	musicOST.pause();
}

void Game::onGameEnd() {

	Save::updateHighscore(points, saveFilePath);

	silenceMusic();
	exitToStartMenu = false;
	if (health <= 0) {

		dyingSound.play();
		//Death menu loop
		while (!exitToStartMenu) {
			update();
			render();
		}
	}
}
void Game::update()
{
    pollEvents();
    updateMousePos();

    if (!endGame) {
        updateUi();
        updateSpeed();
        updateEnemies();
        updateBiteMarks();
        moveHand();
        deleteEnemy();
    }

    if (health <= 0) {
        endGame = true;
    }
}

void Game::render()
{
    // Renders game objects. First clear the last frame, then render objects, then display frame
    window->clear(sf::Color(135, 206, 250));
    window->draw(backGroundObj);
    renderRects(*window);

    if (makeBloodSplatter) {
        renderBlood(*window);
    }
    renderUi(*window);

    if (health <= 0) {
        renderDyingMessage(*window);
    }
    window->display();
}

void Game::spawnEnemy()
{
    static const std::array<sf::Vector2f, 7> enemySizes = {
        sf::Vector2f(75.f, 95.f),
        sf::Vector2f(70.f, 70.f),
        sf::Vector2f(45.f, 90.f),
        sf::Vector2f(50.f, 85.f),
        sf::Vector2f(85.f, 50.f),
        sf::Vector2f(65.f, 30.f),
        sf::Vector2f(55.f, 30.f)
    };

    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> typeDist(0, 6);
    std::uniform_int_distribution<int> coinFlip(0, 1);
    std::uniform_int_distribution<int> speedDist(0, 99);

    const int type = typeDist(rng);
    const sf::Texture& mosquitoPic = coinFlip(rng) ? mosquitoL : mosquitoR;

    const float spawnWidth = static_cast<float>(window->getSize().x) - 50.f;
    const float spawnX = std::uniform_real_distribution<float>(50.f, spawnWidth)(rng);

    Enemy& newEnemy = enemies.emplace_back(
        mosquitoPic,
        sf::Vector2f(spawnX, 0.f),
        std::max(type, 1),   // speed can't be zero
        speedDist(rng) * 10
    );

    newEnemy.setSize(sf::Vector2f(
        0.003f * heightRatio * enemySizes[type].x,
        0.003f * heightRatio * enemySizes[type].y
    ));
}

void Game::pollEvents()
{
    while (window->pollEvent(ev))
    {
        switch (ev.type)
        {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::KeyPressed:
            if (ev.key.code == sf::Keyboard::Escape) {
                toPauseMenu = true;
            } else if (ev.key.code == sf::Keyboard::Enter) {
                exitToStartMenu = true;
            }
            break;
        }
    }
}

void Game::updateMousePos() {
	mousePosWindow = sf::Mouse::getPosition(*window);
	mousePosView = window->mapPixelToCoords(mousePosWindow);
}

void Game::updateUi()
{
    // UI scores
    if (points > highscore) highscore = points;
    scoreText.setString(std::format("Highscore: {}\nPoints: {}\n", highscore, points));

    // Health
    healthBar.setSize(health < 2000
        ? sf::Vector2f(200.f * health / 1000.f, 20.f)
        : sf::Vector2f(200.f, 20.f));

    healthText.setString(std::format("Health: {}", health));

    // Stamina
    const unsigned curStamina = stamina;
    staminaText.setString(std::format("Stamina: {}", curStamina));

    // Stamina bar regen logic
    if (curStamina < 199 && !isStaminaRegen) {
        regenClock.restart();
        isStaminaRegen = true;
    }
    if (curStamina < 199 && regenClock.getElapsedTime().asSeconds() >= 1.f) {
        regenClock.restart();
        staminaBar.setSize(sf::Vector2f(static_cast<float>(curStamina) + 25.f, 20.f));
        stamina += 25;
    }
}
void Game::updateSpeed()
{
    // If not enough points, no need to update
    if (points <= 25 * mltplr) return;

    if (speedY < 2.f)
        speedY *= 1.01f;
    else if (speedY < 2.5f)
        speedY += 0.02f;

    ++mltplr;
}
void Game::deleteEnemy()
{
    // If the player isn't holding the mouse, reset the held flag and bail.
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        mouseHeld = false;
        return;
    }
    // Ignore held-down mouse — only react on the initial press.
    if (mouseHeld) {
        return;
    }
    mouseHeld = true;

    bool hitSkin = false;
    sf::Vector2f bitemarkCoords;

    auto hitIt = std::find_if(enemies.begin(), enemies.end(), [this](const Enemy& enemy) {
        const bool enemyOnHand = mosquitoOnHand(enemy);
        return enemy.getGlobalBounds().contains(mousePosView) && (stamina > 0 || enemyOnHand);
    });

    const bool deleted = (hitIt != enemies.end());

    if (deleted) {
        const bool enemyOnHand = mosquitoOnHand(*hitIt);
        const int scoreAmount = hitIt->getVelocity();

        if (!enemyOnHand) {
            points += static_cast<int>(2.5 * scoreAmount) + 5;
        } else {
            points += 1;
            hitSkin = true;
            sf::FloatRect hitBounds = hitIt->getGlobalBounds();

            bitemarkCoords.x = hitBounds.left + hitBounds.width / 2.0f; 
            bitemarkCoords.y = hitBounds.top + hitBounds.height / 2.0f;

        }
        enemies.erase(hitIt);
    }

    // Hit enemy mid-air
    if (deleted && !hitSkin) {
        hittingInsectSound.play();
        return;
    }

    // Hit mosquito on skin, and had stamina to do it
    if (stamina > 0 && hitSkin) {
        health -= 10;
        makeBloodSplatter = true;
        bloodClock.restart();
        hittingHandSound.play();

        sf::Sprite& newBitemark = bitemarks.emplace_back(sf::Sprite(bloodBitemark));
        newBitemark.setScale(sf::Vector2f(heightRatio * 0.4f, heightRatio * 0.4f));

        sf::FloatRect bounds = newBitemark.getGlobalBounds();
        bitemarkCoords = sf::Vector2f(bitemarkCoords.x - bounds.width/ 2, bitemarkCoords.y - bounds.height/ 2);
        newBitemark.setPosition(bitemarkCoords);
        newBitemark.setColor(sf::Color(255, 255, 255, 128)); 
    }
    // Had stamina, but missed
    else if (stamina > 0 && !hitSkin) {
        isStaminaRegen = false;
        staminaBar.setSize(sf::Vector2f(staminaBar.getSize().x - 25.f, 20.f));
        stamina -= 25;

        if (hairyHand.getGlobalBounds().contains(mousePosView))
            hittingHandSound.play();
        else
            swingingAirSound.play();
    }
}

const bool Game::mosquitoOnHand(const Enemy& enemy) const
{
	return enemy.getPosition().y + enemy.getSize().y >= this->window->getSize().y - this->heightRatio * (60 + 30 * enemy.getVelocity() + enemy.getOffset() / 13);
}

void Game::updateEnemies()
{
	// Check if there's room for more enemies and spawn if necessary
	if (enemies.size() < maxEnemies)
	{
		if (enemySpawnTimer < enemySpawnTimerMax)
		{
			enemySpawnTimer += 1.f;
		}
		else
		{
			spawnEnemy();
			enemySpawnTimer = 0.f;
		}
	}

	// Moving and updating enemies
	std::ranges::for_each(enemies, [this](Enemy& enemy) {
		updateEnemyPosition(enemy);
		checkEnemyBounds(enemy);
	});

	isTouching = false;
}

void Game::updateEnemyPosition(Enemy& enemy)
{
	float enemyX = enemy.getPosition().x;
	float enemyY = enemy.getPosition().y;
	if (!mosquitoOnHand(enemy))
	{
		//If enemy not on hand yet we calculate the trajectory and move it
		float waveX = calculateWaveX(enemyY, enemy.getVelocity(), enemy.getOffset());
		enemy.move(waveX + (2 * (rand() % 2) - 1) * 0.2f, speedY);
		return;
	}
	//If enemy already on hand we take health away
	isTouching = true;
	if (static_cast<int>(rand() % 10) == 0)
	{
		health -= 1;
	}
}

void Game::updateBiteMarks()
{
    std::erase_if(bitemarks,[](auto& mark){
        auto [r,g,b,a] = mark.getColor();
        if (a == 1) {
            return true;
        }
        mark.setColor(sf::Color(r,g,b, a - 1));
        return false;
    } );
}

float Game::calculateWaveX(float enemyY, int velocity, int offset) const
{
	float angle;
	float randomness = static_cast<float>(rand() % 100) / 100;
	if (velocity % 2 == 0)
	{
		angle = (enemyY / 100.0f + offset) * 0.1f;
		return (std::sin(std::sin(angle) * static_cast<float>(std::pow(std::cos(angle), 4)) 
        + std::sin(0.5f * angle) * speedX * 0.1f) * std::sin(angle) * randomness) * widthRatio;
	}
	else
	{
		angle = (enemyY / 110.0f + offset) * 0.1f;
		return (std::sin(angle * static_cast<float>(std::pow(std::cos(angle), 3)) 
        + std::sin(angle) * speedX) * std::cos(angle) * randomness) * widthRatio;
	}
}
void Game::checkEnemyBounds(Enemy& enemy)
{
    const float rightEdge = static_cast<float>(window->getSize().x) - enemy.getSize().x;
    const sf::Vector2f pos = enemy.getPosition();

    if (pos.x > rightEdge) {
        enemy.move(rightEdge - pos.x, 0.f);
    } else if (pos.x < 0.f) {
        enemy.move(-pos.x, 0.f);
    }
}

void Game::moveHand()
{
	if (rand() % 10 == 0)
	{
		float shakeX = (rand() % 3 - 1) * 0.25f;
		float shakeY = (rand() % 3 - 1) * 0.2f;
		hairyHand.move(sf::Vector2f(shakeX, shakeY));
	}
}


void Game::renderBlood(sf::RenderTarget& target)
{
	float elapsedTime = bloodClock.getElapsedTime().asSeconds();
	if (elapsedTime <= 1)
	{
		uint8_t alpha = static_cast<uint8_t>(255 * (1.0f - (elapsedTime / 1)));
		bloodBathObj.setColor(sf::Color(255, 255, 255, alpha));
		target.draw(bloodBathObj);
	}
	else
	{
		makeBloodSplatter = false;
		bloodBathObj.setColor(sf::Color(255, 255, 255, 255));
	}
}

void Game::renderUi(sf::RenderTarget& target)
{
	target.draw(scoreText);
	target.draw(healthBar);
	target.draw(staminaBar);
	target.draw(healthOutline);
	target.draw(staminaOutline);
	target.draw(healthText);
	target.draw(staminaText);
}


void Game::renderRects(sf::RenderTarget& target)
{
	target.draw(hairyHand);
	//render the enemies
	for (auto& enemy : enemies) {
		target.draw(enemy.getEnemySprite());
	}
	for (const auto& bitemark : bitemarks) {
		target.draw(bitemark);
	}
}

void Game::renderDyingMessage(sf::RenderTarget& target) const
{
    target.draw(deathMessage);
}

