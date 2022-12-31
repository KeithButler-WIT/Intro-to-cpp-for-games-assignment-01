#include "zombie.h"
#include "textureHolder.h"
#include <cstdlib>
#include <ctime>
#include <math.h> /* atan2 */

void Zombie::spawn(float startX, float startY, int type, int seed)
{
	switch (type)
	{
		case 0:
			// Bloater
			setSprite(Sprite(TextureHolder::GetTexture("content/graphics/Tiles/tile_0108.png")));
			m_Sprite = getSprite();
			m_Speed = BLOATER_SPEED;
			m_Health = BLOATER_HEALTH;
			break;
		case 1:
			// Chaser
			setSprite(m_Sprite = Sprite(TextureHolder::GetTexture("content/graphics/Tiles/tile_0109.png")));
			m_Sprite = getSprite();
			m_Speed = CHASER_SPEED;
			m_Health = CHASER_HEALTH;
			break;
		case 2:
			// Crawler
			setSprite(Sprite(TextureHolder::GetTexture("content/graphics/Tiles/tile_0110.png")));
			m_Sprite = getSprite();
			m_Speed = CRAWLER_SPEED;
			m_Health = CRAWLER_HEALTH;
			break;
		default:
			break;
	}

	// Prevent zombies from bunching
	srand((int)time(0) * seed);
	// Somewhere between 70 and 100
	float modifier = (rand() % MAX_VARIANCE) + OFFSET;
	// Express as a fraction of 1
	modifier /= 100; // Now equals between .7 and 1
	m_Speed *= modifier;

	m_Position.x = startX;
	m_Position.y = startY;

	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
}

bool Zombie::hit()
{
	m_Health--;

	if (m_Health < 0)
	{
		// if the zombie is dead
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("content/graphics/Tiles/tile_0064.png"));

		return true;
	}

	// injured but not dead yet
	return false;
}

bool Zombie::isAlive()
{
	return m_Alive;
}

void Zombie::update(float elapsedTime, Vector2f playerLocation)
{
	float playerX = playerLocation.x + 16.0f;
	float playerY = playerLocation.y + 16.0f;

	// Update the zombie position variables
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x + m_Speed * elapsedTime;
	}

	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y + m_Speed * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x - m_Speed * elapsedTime;
	}

	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y - m_Speed * elapsedTime;
	}

	// Move the sprite
	m_Sprite.setPosition(m_Position);

	// Face the sprite in the correct direction
	// float angle = (atan2(playerY - m_Position.y,
	// 				   playerX - m_Position.x)
	// 				  * 180)
	// 	/ 3.141;

	// m_Sprite.setRotation(angle);
}
