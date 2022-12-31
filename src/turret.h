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

	// An array of bullets.  100 bullets should do
	Bullet m_Bullets[100];
	int m_CurrentBullet = 0;
	int m_BulletsSpare = START_MAX_BULLETS;
	int m_FireRate = 1;

	bool m_IsPlaced = false;

	Time currentShotTime;
	Time lastShotTime;

	// What is the screen resolution?
	Vector2f m_Resolution;

	// Damage the turret can do
	float m_Damage;

public:
	Turret();

	void spawn(Vector2f playerPosition, Vector2f resolution);

	// Called at end of game
	void resetTurretStats();

	// Called every frame
	// void update(Vector2i targetPosition);
	void update(Vector2f targetPosition);

	float getDamage();

	// Returns a bullet at the index bulletNum
	Bullet getBullet(int bulletNum);

	// Increase damage used for levelling up
	void upgradeDamage();

	// Reloads the turret by resetting the current bullet to 0 and spare to max
	void resetBullets();
};

#endif // TURRET_H_
