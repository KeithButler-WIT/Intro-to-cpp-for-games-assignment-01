#ifndef turret_h
#define turret_h

#include "bullet.h"
#include "entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Turret : virtual public Entity, virtual public Bullet
{
private:
	const float START_DAMAGE = 1;
	const int START_MAX_BULLETS = 99;
	const int START_FIRE_RATE = 1;

	// An array of bullets
	Bullet m_Bullets[100];
	int m_CurrentBullet = 0;
	int m_BulletsSpare;
	int m_FireRate;

	bool m_IsPlaced = false;

	// When the last shot was taken
	Time lastShotTime;

	// What is the screen resolution?
	Vector2f m_Resolution;

	// Damage the turret can do
	float m_Damage;

public:
	Turret();

	// void spawn(Vector2f playerPosition, Vector2f resolution);
	void spawn(Vector2f playerPosition, Vector2f resolution);

	// Called at end of game
	void resetTurretStats();

	// Called every frame
	// void update(Vector2i targetPosition);
	void update(Time gameTimeTotal, Vector2f targetPosition);

	float getDamage();

	// Returns a bullet at the index bulletNum
	Bullet getBullet(int bulletNum);

	// Increase damage used for levelling up
	void upgradeDamage();

	// Increase fireRate used for levelling up
	void upgradeFireRate();

	// Reloads the turret by resetting the current bullet to 0 and spare to max
	void resetBullets();
};

#endif // TURRET_H_
