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
FloatRect Entity::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

// Where is the centre of the entity?
Vector2f Entity::getCenter()
{
	return m_Position;
}
// Which angle is the entity facing?

float Entity::getRotation()
{
	return m_Sprite.getRotation();
}
// Send a copy of the sprite to main
Sprite Entity::getSprite()
{
	return m_Sprite;
}

void Entity::setSprite(Sprite sprite)
{
	//TODO error checking etc
	m_Sprite = sprite;
}
