#ifndef TURRET_H_
#define TURRET_H_

#include <SFML/Graphics.hpp>

using namespace sf;

class Entity
{
protected:
	// Where is the entity?
	Vector2f m_Position;

	// Entity sprite
	Sprite m_Sprite;

	// Entity texture
	Texture m_Texture;

	// Speed in pixels per second
	float m_Speed;

	// True if collision happened
	bool m_Collision;

public:
	Entity();

	// Where is the player?
	FloatRect getPosition() const;
	//
	// void setPosition(Vector2f position);
	// Where is the centre of the player?
	Vector2f getCenter() const;
	// Which angle is the player facing?
	float getRotation() const;
	// Send a copy of the sprite to main
	Sprite getSprite() const;
	// Set the sprite
	void setSprite(Sprite sprite);

	// Checks for collision with an object of type Entity
	bool checkCollision(Entity Obj);

	// checks if colliding with any object
	bool isColliding();
};

#endif // TURRET_H_
