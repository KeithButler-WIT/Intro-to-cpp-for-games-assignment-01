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

public:
	Entity();

	// Where is the player?
	FloatRect getPosition();
	// Where is the centre of the player?
	Vector2f getCenter();
	// Which angle is the player facing?
	float getRotation();
	// Send a copy of the sprite to main
	Sprite getSprite();
	// Set the sprite
	void setSprite(Sprite sprite);
};

#endif // TURRET_H_
