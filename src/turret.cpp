#include "turret.h"
#include "textureHolder.h"
#include <math.h> /* atan2 */

Turret::Turret()
{
	m_Damage = START_DAMAGE;
	m_BulletsSpare = START_MAX_BULLETS;
	m_FireRate = START_FIRE_RATE;

	// Associate a texture with the sprite
	Entity::setSprite(Sprite(TextureHolder::GetTexture("content/graphics/Tiles/tile_0074.png")));
	Entity::m_Sprite = Entity::getSprite();

	// Set the origin of the sprite to the centre for smooth rotation
	Entity::m_Sprite.setOrigin(8, 8);
}

void Turret::spawn(Vector2f playerPosition)
{
	// If turret already placed don't move it
	// if (m_IsPlaced)
	// {
	// 	return;
	// }

	// Place the turret at the players position
	Entity::m_Position.x = playerPosition.x;
	Entity::m_Position.y = playerPosition.y;

	m_Sprite.setPosition(Entity::m_Position);

	m_IsPlaced = true;
}

void Turret::resetTurretStats()
{
	m_Damage = START_DAMAGE;
	m_CurrentBullet = 0;
	m_BulletsSpare = START_MAX_BULLETS;
	m_FireRate = START_FIRE_RATE;
}

// void Turret::update(Vector2i targetPosition)
void Turret::update(Vector2f targetPosition)
{
	// Calculate the angle to face the enemy
	float angle = (atan2(targetPosition.y - m_Resolution.y / 2,
					   targetPosition.x - m_Resolution.x / 2)
					  * 180)
		/ 3.141;

	Entity::m_Sprite.setRotation(angle);

	if (currentShotTime.asMilliseconds() - lastShotTime.asMilliseconds() > 1000 / m_FireRate && m_BulletsSpare > 0)
	{
		// Pass the centre of the turret and the centre of the target to the shoot function
		m_Bullets[m_CurrentBullet].shoot(Entity::getCenter().x, Entity::getCenter().y, targetPosition.x, targetPosition.y);
		m_Bullets[m_CurrentBullet].getSprite().setRotation(angle);
		m_CurrentBullet++;

		if (m_CurrentBullet > START_MAX_BULLETS)
		{
			m_CurrentBullet = 0;
		}

		lastShotTime = currentShotTime;
		m_BulletsSpare--;
	}

	Entity::m_Sprite.setPosition(Entity::m_Position);
}

float Turret::getDamage()
{
	return m_Damage;
}

Bullet Turret::getBullet(int bulletNum)
{
	// TODO error checking
	return m_Bullets[bulletNum];
}

void Turret::upgradeDamage()
{
	// 20% damage upgrade
	m_Damage += (START_DAMAGE * .2);
}

void Turret::upgradeFireRate()
{
	// 2x fireRate upgrade
	m_FireRate += (START_FIRE_RATE * 2);
}

void Turret::resetBullets()
{
	m_CurrentBullet = 0;
	m_BulletsSpare = START_MAX_BULLETS;
}
