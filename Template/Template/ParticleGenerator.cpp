#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
	:shader(shader) , texture(texture) , amount(amount)
{
	this->init();
}


void ParticleGenerator::Update(GLfloat dt, GameObject &object, GLuint newParticles, vec2 offset)
{
	//Add new particles

	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	//update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= dt;	//reduce life
		if (p.Life > 0.0f)
		{
			//particle is alive thus update
			p.Position -= p.Velocity * dt;
			//make it more transparent
			p.Color[3] -= dt * 2.5;
		}
	}
}


//Render particles
void ParticleGenerator::Draw()
{
	//use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.use();

	for (Particle particle : this->particles)
	{
		//if alive
		if (particle.Life > 0.0f)
		{
			this->shader.setVec2("offset", particle.Position);
			this->shader.setVec4("color", particle.Color);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	//reset the default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	//set up mesh and attribute properties
	GLuint VBO;

	GLfloat particle_quad[] = 
	{
		0.0f,1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,

		0.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
		1.0f,0.0f,1.0f,0.0f
	};
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	//set mesh attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; i++)
	{
		this->particles.push_back(Particle());
	}
}


//Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	//First search from the last unused particle , this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i)
	{
		if (this->particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	// otherwise do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i)
	{	
		if (this->particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	//All particles are taken , override the first one (note that if it repeatedly hits this case, more particles
	//should be reserved
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat color = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = vec4(color, color, color, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}



















