#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const sf::Texture& texture, const sf::Vector2f& position, int velocity, int offset)
{
	enemyObj.setTexture(texture);
	enemyObj.setPosition(position);
	this->velocity = velocity;
	this->offset = offset;
}

void Enemy::move(const float& speedX, const float& speedY)
{
	enemyObj.move(sf::Vector2f(speedX, speedY*velocity*0.1f));
}

void Enemy::setPosition(const sf::Vector2f& position)
{
	enemyObj.setPosition(position);
}

void Enemy::setSize(sf::Vector2f size)
{
	enemyObj.setScale(size);
}

int Enemy::getVelocity() const
{
	return this->velocity;
}

int Enemy::getOffset() const
{
	return this->offset;
}

sf::FloatRect Enemy::getGlobalBounds() const
{
	return enemyObj.getGlobalBounds();
}

sf::Vector2f Enemy::getSize() const
{
	sf::FloatRect bounds = enemyObj.getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

sf::Vector2f Enemy::getPosition() const
{
	return enemyObj.getPosition();
}

sf::Sprite Enemy::getEnemySprite() const
{
	return this->enemyObj;
}

