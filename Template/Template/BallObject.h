#pragma once
#include "GameObject.h"


class BallObject : public GameObject
{
public:
	//ball state
	GLfloat radius;
	GLboolean Stuck;

	BallObject();
	BallObject(vec2 pos, GLfloat radius, vec2 velocity, Texture2D sprite);

	vec2 Move(GLfloat dt, GLuint window_width);
	void Reset(vec2 position, vec2 velocity);
};
