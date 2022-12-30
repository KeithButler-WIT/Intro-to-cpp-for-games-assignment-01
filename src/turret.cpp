#include "turret.h"
#include "textureHolder.h"
#include <math.h> /* atan2 */

Turret::Turret()
{

	m_Damage = START_DAMAGE;

	// Associate a texture with the sprite
	setSprite(Sprite(TextureHolder::GetTexture("content/graphics/Tiles/tile_0074.png")));
	m_Sprite = getSprite();

	// Set the origin of the sprite to the centre for smooth rotation
	m_Sprite.setOrigin(8, 8);
}

void Turret::spawn(Vector2f playerPosition, Vector2f resolution)
{
	// Place the turret at the players position
	Entity::m_Position.x = playerPosition.x;
	Entity::m_Position.y = playerPosition.y;

	// setPosition(m_Position);

	// Store the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

void Turret::resetTurretStats()
{
	m_Damage = START_DAMAGE;
}

// void Turret::update(float elapsedTime, Vector2i mousePosition)
void Turret::update(Vector2i mousePosition, Vector2f playerPosition)
{
	m_Sprite.setPosition(Entity::m_Position);

	// Calculate the angle to face the enemy
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
					   mousePosition.x - m_Resolution.x / 2)
					  * 180)
		/ 3.141;

	m_Sprite.setRotation(angle);

	if (currentShotTime.asMilliseconds() - lastShotTime.asMilliseconds() > 1000 / m_FireRate && m_BulletsSpare > 0)
	{
		if (m_BulletsSpare > 0)
		{
			// Pass the centre of the turret and the centre of the target to the shoot function
			m_Bullets[m_CurrentBullet].shoot(getCenter().x, getCenter().y, playerPosition.x, playerPosition.y);
			m_CurrentBullet++;

			if (m_CurrentBullet > 99)
			{
				m_CurrentBullet = 0;
			}

			lastShotTime = currentShotTime;
			m_BulletsSpare--;
		}
	}
}

void Turret::upgradeDamage()
{
	m_Damage *= 2;
}

float Turret::getDamage()
{
	return m_Damage;
}

Bullet Turret::getBullet(int bulletNum)
{
	// if (bulletNum > 99 || bulletNum < 0)
	// 	return Null;
	// else
	return m_Bullets[bulletNum];
}
