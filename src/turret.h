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

	// An array of bullets.  100 bullets should do
	Bullet m_Bullets[100];
	int m_BurrentBullet = 0;
	int m_BulletsSpare = 90;
	int m_CurrentBullet = 0;
	int m_FireRate = 1;

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
	// void update(float elapsedTime, Vector2i mousePosition);
	void update(Vector2f targetPosition, Vector2f playerPosition);

	// Give turret a damage boost used for levelling up
	void upgradeDamage();

	float getDamage();

	// Returns a bullet at the index bulletNum
	Bullet getBullet(int bulletNum);
};

#endif // TURRET_H_
