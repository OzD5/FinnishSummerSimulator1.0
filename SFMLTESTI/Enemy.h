#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Enemy
{
private:
	sf::Sprite enemyObj;
	int velocity;
	int offset;
public:
	//Constructor
	Enemy(const sf::Texture& texture, const sf::Vector2f& position, int velocity, int offset);

	//Input functions

	void move(const float& speedX, const float& speedY);

	void setPosition(const sf::Vector2f& position);

	void setSize(sf::Vector2f size);

	//Output functions

	int getVelocity() const;

	int getOffset() const;

	sf::FloatRect getGlobalBounds() const;
	
	sf::Vector2f getSize() const;

	sf::Vector2f getPosition() const;

	sf::Sprite getEnemySprite() const;


};