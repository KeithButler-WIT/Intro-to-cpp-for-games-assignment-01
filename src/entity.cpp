#include "entity.h"
#include "textureHolder.h"

Entity::Entity()
{
	// Associate a texture with the sprite
	setSprite(Sprite(TextureHolder::GetTexture("content/graphics/Tiles/tile_0061.png")));
	m_Sprite = getSprite();
	// Set the origin of the sprite to the centre for smooth rotation
	m_Sprite.setOrigin(8, 8);
}

// Where is the entity?
FloatRect Entity::getPosition() const
{
	return m_Sprite.getGlobalBounds();
}

// void Entity::setPosition(Vector2f position)
// {
// 	m_Position = position;
// }

// Where is the centre of the entity?
Vector2f Entity::getCenter() const
{
	return m_Position;
}
// Which angle is the entity facing?

float Entity::getRotation() const
{
	return m_Sprite.getRotation();
}
// Send a copy of the sprite to main
Sprite Entity::getSprite() const
{
	return m_Sprite;
}

void Entity::setSprite(Sprite sprite)
{
	//TODO error checking etc
	m_Sprite = sprite;
}

// return true if intersecting with an entity
bool Entity::checkCollision(Entity obj)
{
	m_Collision = false; // defaults to no collision at first
	if (m_Sprite.getGlobalBounds().intersects(obj.getPosition()))
	{
		m_Collision = true;
		return true;
	}

	return false;
}

// TODO
// BUG Error prone
bool Entity::isColliding()
{
	return m_Collision;
}
