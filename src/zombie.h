//
//  zombie.h
//  Zombie Arena
//
//

#ifndef zombie_hpp
#define zombie_hpp

#include "entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie : public Entity
{
private:
	// How fast is each zombie type?
	const float BLOATER_SPEED = 30;
	const float CHASER_SPEED = 60;
	const float CRAWLER_SPEED = 10;

	// How tough is each zombie type
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	// Make each zombie vary its speed slightly
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	// How much health has it got?
	float m_Health;

	// Is it still alive?
	bool m_Alive = true;

	// Public prototypes go here
public:
	// Handle when a bullet hits a zombie
	bool hit();

	// Find out if the zombie is alive
	bool isAlive();

	// Spawn a new zombie
	void spawn(float startX, float startY, int type, int seed);

	// Update the zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);
};

#endif /* zombie_hpp */
