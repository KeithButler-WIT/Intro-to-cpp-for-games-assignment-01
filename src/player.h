#ifndef player_hpp
#define player_hpp

#include "entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Player : public Entity
{
private:
	const float START_SPEED = 100;
	const float START_DASH_SPEED = 600;
	const float START_HEALTH = 100;
	const int START_LEVEL = 1;

	// What is the screen resolution?
	Vector2f m_Resolution;

	// How big is the current arena?
	IntRect m_Arena;

	// How big is each tile of the arena?
	int m_TileSize;

	// Which directions is the player currently moving in?
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	// Check if the player is dashing
	bool m_DashPressed;

	// How much health has the player got?
	int m_Health;
	// What is the maximum health the player can have?
	int m_MaxHealth;

	// When was the player last hit?
	Time m_LastHit;

	// Speed in pixels per second
	float m_Speed;

	// Dash distance in pixels per second
	float m_DashSpeed;

	// Number of times the player leveled up
	int m_Level;

public:
	Player();

	void spawn(IntRect& arena, Vector2f resolution, int tileSize);

	// Called at end of game
	void resetPlayerStats();
	// Handle the player getting hit by a zombie
	bool hit(Time timeHit);
	// check if the player is dead or not
	bool isAlive();
	// How long since player hit?
	Time getLastHitTime();

	// How much health has the player currently got?
	int getHealth();

	// Move the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	// Stop the player moving in a specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	// Jump the player a set distance in the movement direction
	void startDash();
	void stopDash();
	bool isDashing();

	// Called every frame
	// void update(float elapsedTime, Vector2i mousePosition);
	void update(float elapsedTime);

	// Increase dash distance used for levelling up
	void upgradeDash();
	// Give player a speed boost used for levelling up
	void upgradeSpeed();
	// Increase max health  used for levelling up
	void upgradeHealth();
	// Increase health  collision with med kit
	void increaseHealthLevel(int amount);
	// adds 1 to the players level
	void increaseLevel();
	int getLevel() const;
};

#endif /* player_hpp */
