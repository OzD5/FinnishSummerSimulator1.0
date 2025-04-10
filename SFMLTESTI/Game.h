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
	//Always private variables
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode videoMode;

	//Mouse position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	//Recources
	sf::Font font;

	sf::Text uiText;
	sf::RectangleShape healthBar;
	sf::RectangleShape staminaBar;

	//Sounds
	sf::Music musicOST;

	sf::SoundBuffer hittingHandFile;
	sf::Sound hittingHandSound;

	sf::SoundBuffer swingingAirFile;
	sf::Sound swingingAirSound;

	sf::SoundBuffer hittingInsectFile;
	sf::Sound hittingInsectSound;
	// Game Logic
	bool isMiss;
	bool isTouching;
	bool endGame;
	bool isStaminaRegen;


	short difficulty;
	unsigned int highscore;
	int points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	long long health;
	bool mouseHeld;
	float speedX;
	float speedY;
	int mltplr;

	sf::Clock regenClock;

	//Game objects
	sf::Texture handTexture;
	sf::Sprite hairyHand;
	std::vector < Enemy > enemies;

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
	void moveEnemies();
	void moveHand();

	void renderBlood(sf::RenderTarget& target);
	void renderUi(sf::RenderTarget& target);
	void renderRects(sf::RenderTarget& target);
public:
	//construction / destruction
	Game(short difficulty);
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	void update();
	void render();
};