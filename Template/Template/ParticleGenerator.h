#pragma once
#include <vector>
#include "shader.h"
#include "Texture2D.h"
#include "GameObject.h"

using vmath::vec4;

//particle layout 

struct Particle
{
	vec2 Position, Velocity;
	vec4 Color;
	GLfloat Life;
	
	Particle() : Position(0.0f) , Velocity(0.0f) ,Color(1.0f) , Life(0.0f) { }
};


//here we keep particles inside a container
// we spawn , update and kill them in a time span

class ParticleGenerator
{
public:
	//constructor
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	
	//Update all particles
	void Update(GLfloat dt, GameObject &object, GLuint newParticles, vec2 offset = vec2(0.0f, 0.0f));
	//Render all particles
	void Draw();

private:
	//State
	std::vector<Particle>particles;
	GLuint amount;
	//Render state 
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	//Intialize buffer and vertex attributes
	void init();
	//return first particle index which is currently unused
	GLuint firstUnusedParticle();
	//respawn particle
	void respawnParticle(Particle &particle, GameObject &object, vec2 offset = vec2(0.0f, 0.0f));
};











