#pragma once
#include "Sysfiles.h"
#include "vmath.h"
#include "glFiles.h"
#include "Texture2D.h"
#include "SpriteRenderer.h"

using vmath::vec2;
using vmath::vec3;


class GameObject
{
public:
	//object attribs
	vec2 Position, Size, Velocity;
	vec3 Color;
	GLfloat Rotation;
	GLboolean Destroyed;
	GLboolean IsSolid;

	//Render state
	Texture2D Sprite;
	
	//Constructor
	GameObject();
	GameObject(vec2 pos, vec2 size, Texture2D sprite, vec3 color = vec3(1.0f), vec2 velocity = vec2(0.0f, 0.0f));

	//Draw Sprites
	virtual void Draw(SpriteRenderer &renderer);
};
