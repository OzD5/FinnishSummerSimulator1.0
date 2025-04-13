#pragma once

#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include "Enemy.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
/*
* "Game enginena" toimiva 
*/
class Game
{
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
	// Game Logic
	short difficulty;
	unsigned windowHeight;
	unsigned windowWidth;

	float heightRatio;
	float widthRatio;

	bool endGame;
	unsigned int highscore;
	unsigned int points;

	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	long long health;


	bool mouseHeld;
	bool isTouching;
	bool isMiss;
	bool isStaminaRegen;
	
	float speedX;
	float speedY;
	unsigned int mltplr;

	sf::Clock regenClock;

	//Game objects
	sf::Texture handTexture;
	sf::Sprite hairyHand;
	std::vector<Enemy> enemies;

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



	void initVariables();
	void initWindow();
	void initFonts();
	void initUi();
	void initText(sf::Text& textObj, std::string text, int size, float x, float y) const;
	void initBar(sf::RectangleShape& bar, float x, float y, sf::Color color) const;
	void initOutline(sf::RectangleShape& outline, float x, float y) const;
	void initGraphics();
	void initSounds();
	void initHand();

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
public:
	//construction / destruction
	Game(short difficulty, unsigned windowWidth, unsigned windowHeight);
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	void update();
	void render();
};