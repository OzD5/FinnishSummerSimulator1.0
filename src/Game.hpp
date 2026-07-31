#pragma once

#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <memory>

#include "Enemy.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class GameInitializer;
/*
* "Game engine"
*/
class Game
{
public:
	Game(short difficulty, unsigned windowWidth, unsigned windowHeight, sf::RenderWindow* window);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	virtual ~Game();

	//Accessors
	bool running() const;
	bool getEndGame() const;
	bool getPauseMenu() const;
	void silenceMusic();
	void onGameEnd();

	void update();
	void render();
private:
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode videoMode;

	//Mouse position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	//Recources
	sf::Font font;

	sf::Text scoreText;

	sf::Text healthText;
	sf::Text staminaText;
	sf::Text deathMessage;
	sf::RectangleShape healthBar;
	sf::RectangleShape staminaBar;

	sf::RectangleShape healthOutline;
	sf::RectangleShape staminaOutline;

	//Sounds
	sf::Music musicOST;

	sf::SoundBuffer hittingHandFile;
	sf::Sound hittingHandSound;

	sf::SoundBuffer swingingAirFile;
	sf::Sound swingingAirSound;

	sf::SoundBuffer hittingInsectFile;
	sf::Sound hittingInsectSound;

	sf::SoundBuffer dyingSoundFile;
	sf::Sound dyingSound;

	std::string saveFilePath;
	// Game Logic
	short difficulty;
	unsigned windowHeight;
	unsigned windowWidth;

	float heightRatio = 0.0f;
	float widthRatio = 0.0f;

	bool toPauseMenu = false;
	bool endGame = false;
	bool exitToStartMenu = false;
	unsigned int highscore = 0;
	unsigned int points = 0;

	float enemySpawnTimer = 0.0f;
	float enemySpawnTimerMax = 0.0f;
	std::size_t maxEnemies = 0;
	long long health = 0;
	unsigned int stamina = 0;


	bool mouseHeld = false;
	bool isTouching = false;
	bool makeBloodSplatter = false;
	bool isStaminaRegen = false;
	
	float speedX = 0.0f;
	float speedY = 0.0f;
	unsigned int mltplr = 0;

	sf::Clock regenClock;

	//Game objects
	sf::Texture handTexture;
	sf::Sprite hairyHand;
	std::vector<Enemy> enemies;
	std::vector<sf::Sprite> bitemarks;

	//Graphics
	sf::Texture backGround;
	sf::Sprite backGroundObj;
	//Enemy object uses
	sf::Texture mosquitoL;
	sf::Texture mosquitoR;
	//Blood Objects
	sf::Texture bloodBath;
	sf::Sprite bloodBathObj;
	sf::Clock bloodClock;
	sf::Texture bloodBitemark;

	//Game Initializer can access private variables of Game class
	friend class GameInitializer;


	//Functions

	void spawnEnemy();

	void pollEvents();
	void updateMousePos();
	void updateUi();
	void updateSpeed();
	void deleteEnemy();
	const bool mosquitoOnHand(const Enemy& enemy) const;
	void updateEnemies();
	void updateEnemyPosition(Enemy& enemy);
	float calculateWaveX(float enemyY, int velocity, int offset) const; 
	void checkEnemyBounds(Enemy& enemy);
	void moveHand();

	void renderBlood(sf::RenderTarget& target);
	void renderUi(sf::RenderTarget& target);
	void renderRects(sf::RenderTarget& target);
	void renderDyingMessage(sf::RenderTarget& target) const;
};