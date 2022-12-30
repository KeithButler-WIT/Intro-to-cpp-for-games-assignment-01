#ifndef turret_h
#define turret_h

#include "bullet.h"
#include "entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Turret : public Entity, public Bullet
{
private:
	const float START_DAMAGE = 1;

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
	void update(Vector2i mousePosition);

	// Give turret a damage boost used for levelling up
	void upgradeDamage();

	float getDamage();
};

#endif // TURRET_H_
