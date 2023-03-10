#include "player.h"
#include "textureHolder.h"
#include <math.h> /* atan2 */

Player::Player()
{
	m_Speed = START_SPEED;
	m_DashSpeed = START_DASH_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
	m_Level = START_LEVEL;

	m_DashPressed = false;

	// Associate a texture with the sprite
	setSprite(Sprite(TextureHolder::GetTexture("content/graphics/Tiles/tile_0084.png")));
	m_Sprite = getSprite();

	// Set the origin of the sprite to the centre for smooth rotation
	m_Sprite.setOrigin(8, 8);
}

void Player::spawn(IntRect& arena, Vector2f resolution, int tileSize)
{
	// Place the player in the middle of the arena
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	// Copy the details of the arena to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;

	m_Arena.height = arena.height;

	// Remember how big the tiles are in this arena
	m_TileSize = tileSize;

	// Store the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
	m_DashSpeed = START_DASH_SPEED;
	m_Level = START_SPEED;
}

Time Player::getLastHitTime()
{
	return m_LastHit;
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::isAlive()
{
	if (m_Health > 0)
		return true;

	return false;
}

int Player::getHealth()
{
	return m_Health;
}

void Player::moveLeft()
{
	m_LeftPressed = true;
}

void Player::moveRight()
{
	m_RightPressed = true;
}
void Player::moveUp()
{
	m_UpPressed = true;
}

void Player::moveDown()
{
	m_DownPressed = true;
}

void Player::stopLeft()
{
	m_LeftPressed = false;
}
void Player::stopRight()
{
	m_RightPressed = false;
}
void Player::stopUp()
{
	m_UpPressed = false;
}
void Player::stopDown()
{
	m_DownPressed = false;
}

void Player::startDash()
{
	m_DashPressed = true;
}
void Player::stopDash()
{
	m_DashPressed = false;
}

bool Player::isDashing()
{
	return m_DashPressed;
}

// void Player::update(float elapsedTime, Vector2i mousePosition)
void Player::update(float elapsedTime)
{
	if (m_UpPressed)
	{
		if (m_DashPressed)
			m_Position.y -= m_DashSpeed * elapsedTime;
		else
			m_Position.y -= m_Speed * elapsedTime;
	}

	if (m_DownPressed)
	{
		if (m_DashPressed)
			m_Position.y += m_DashSpeed * elapsedTime;
		else
			m_Position.y += m_Speed * elapsedTime;
	}

	if (m_RightPressed)
	{
		if (m_DashPressed)
			m_Position.x += m_DashSpeed * elapsedTime;
		else
			m_Position.x += m_Speed * elapsedTime;
	}

	if (m_LeftPressed)
	{
		if (m_DashPressed)
			m_Position.x -= m_DashSpeed * elapsedTime;
		else
			m_Position.x -= m_Speed * elapsedTime;
	}

	m_Sprite.setPosition(m_Position);

	// Keep the player in the arena
	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}

	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}
	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}

	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}

	// Calculate the angle the player is facing
	// float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
	// 				   mousePosition.x - m_Resolution.x / 2)
	// 				  * 180)
	// 	/ 3.141;

	// m_Sprite.setRotation(angle);
}

void Player::upgradeDash()
{
	// 20% speed upgrade
	m_DashSpeed += (START_DASH_SPEED * .2);
}

void Player::upgradeSpeed()
{
	// 20% speed upgrade
	m_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth()
{
	// 20% max health upgrade
	m_MaxHealth += (START_HEALTH * .2);
}

void Player::increaseHealthLevel(int amount)
{
	m_Health += amount;

	// But not beyond the maximum
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

void Player::increaseLevel()
{
	m_Level++;
}

int Player::getLevel() const
{
	return m_Level;
}
