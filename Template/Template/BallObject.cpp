#include "BallObject.h"

BallObject::BallObject() : GameObject(), radius(12.5f), Stuck(true) {}

BallObject::BallObject(vec2 pos, GLfloat radius, vec2 velocity, Texture2D sprite) 
	: GameObject(pos, vec2(radius * 2, radius * 2), sprite, vec3(1.0f), velocity), radius(radius), Stuck(true) {}


//if not stuck to players board then move the damn thing
vec2 BallObject::Move(GLfloat dt, GLuint window_width)
{
	if (!this->Stuck)
	{
		//move
		this->Position += this->Velocity * dt;

		if (this->Position[0] <= 0.0f)
		{
			this->Velocity[0] = -this->Velocity[0];
			this->Position[0] = 0.0f;
		}
		else if (this->Position[0] + this->Size[0] >= window_width)
		{
			this->Velocity[0] = -this->Velocity[0];
			this->Position[0] = window_width - this->Size[0];
		}

		if (this->Position[1] <= 0.0f)
		{
			this->Velocity[1] = -this->Velocity[1];
			this->Position[1] = 0.0f;
		}
	}

	return this->Position;
}


void BallObject::Reset(vec2 position, vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = true;
}


