#ifndef TURRET_H_
#define TURRET_H_

#include "entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

// class Turret : public Entity
// {
// private:
// 	// What is the screen resolution?
// 	Vector2f m_Resolution;

// 	// How big is the current arena?
// 	IntRect m_Arena;

// 	// How big is each tile of the arena?
// 	int m_TileSize;

// public:
// 	Turret();

// 	void spawn(IntRect arena, Vector2f resolution, int tileSize);

// 	// Called at end of game
// 	void resetTurretStats();

// 	// Called every frame
// 	void update(float elapsedTime);

// 	// Give turret a damage boost used for levelling up
// 	void upgradeDamage();
// };

// // Calculate the angle to face the enemy
// float angle = (atan2(targetPosition.y - m_Resolution.y / 2,
// 				   targetPosition.x - m_Resolution.x / 2)
// 				  * 180)
// 	/ 3.141;

// m_Sprite.setRotation(angle);

#endif // TURRET_H_
