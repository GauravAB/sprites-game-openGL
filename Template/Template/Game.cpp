#include "Game.h"

extern float timer;


//Initial size of player paddle
const vec2 PLAYER_SIZE(200, 30);
//Initial velocity of player paddle
const GLfloat PLAYER_VELOCITY(500.0f);

//Initialize velcity of ball
const vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 17.5f;


SpriteRenderer *Renderer;
GameObject *Player;
BallObject *Ball;
PostProcessor *Effects;
GLfloat ShakeTime = 0.0f;
ParticleGenerator *Particles;


Game::Game( GLuint width , GLuint height) : State(GAME_ACTIVE) , Keys() , Width(width) , Height(height)
{
	
}

Game :: ~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
}


void Game::Initialize()
{
	ResourceManager::LoadShader("Shaders/spriteVertexShader.glsl", NULL, NULL, NULL, "Shaders/spriteFragmentShader.glsl", "Sprite");
	ResourceManager::LoadShader("Shaders/particleVertexShader.glsl", NULL, NULL, NULL, "Shaders/particleFragmentShader.glsl", "particle");
	ResourceManager::LoadShader("Shaders/postProcessingVertexShader.glsl", NULL, NULL, NULL, "Shaders/postProcessingFragmentShader.glsl", "postprocessing");

	//configure the shaders
	vmath::mat4 projection_matrix = vmath::ortho(0.0f, static_cast<GLfloat>(this->Width),static_cast<GLfloat>(this->Height),0.0f, -1.0f, 1.0f);
	
	ResourceManager::GetShader("Sprite").use();
	ResourceManager::GetShader("Sprite").setInt("image",0);
	ResourceManager::GetShader("Sprite").setMat4("u_projection_matrix",projection_matrix,GL_FALSE);

	ResourceManager::GetShader("particle").use();
	ResourceManager::GetShader("particle").setMat4("projection", projection_matrix, GL_FALSE);

	ResourceManager::GetShader("postprocessing").use();
	ResourceManager::GetShader("postprocessing").setMat4("u_projection_matrix", projection_matrix, GL_FALSE);

	
	//Load textures
	ResourceManager::LoadTexture("resources/awsomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("resources/background.jpg",GL_TRUE,"background");
	ResourceManager::LoadTexture("resources/block.png", GL_TRUE, "block");
	ResourceManager::LoadTexture("resources/block_solid.png", GL_TRUE, "block_solid");
	ResourceManager::LoadTexture("resources/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("resources/particle.png", GL_TRUE, "particle");

	//set renderer specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("Sprite"));

	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);

	Particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		500);

	//Load Levels
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height* 0.5);
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height* 0.5);
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height* 0.5);
	
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	//0 - 3
	this->Level = 2;

	vec2 playerPos = vec2(this->Width / 2 - PLAYER_SIZE[0] / 2, this->Height - PLAYER_SIZE[1]);

	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	vec2 ballPos = playerPos + vec2(PLAYER_SIZE[0] / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY , ResourceManager::GetTexture("face"));
}


void Game::Uninitialize()
{

}

void Game::ResetLevel()
{
	if (this->Level == 0)
	{
		this->Levels[0].Load("Levels/one.lvl", this->Width, this->Height * 0.5f);
	}
	else if (this->Level == 1)
	{
		this->Levels[1].Load("Levels/two.lvl", this->Width, this->Height * 0.5f);
	}
	else if (this->Level == 2)
	{
		this->Levels[2].Load("Levels/three.lvl", this->Width, this->Height * 0.5f);
	}
	else if (this->Level == 3)
	{
		this->Levels[3].Load("Levels/four.lvl", this->Width, this->Height * 0.5f);
	}


}


void Game::ResetPlayer()
{
	Player->Size = PLAYER_SIZE;
	Player->Position = vec2(this->Width / 2 - PLAYER_SIZE[0] / 2, this->Height - PLAYER_SIZE[1]);
	Ball->Reset(Player->Position + vec2(PLAYER_SIZE[0] / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}


void Game::Update(GLfloat dt)
{
	Ball->Move(dt, this->Width);
	this->DoCollisions();

	Particles->Update(dt, *Ball, 2, vec2(Ball->radius) / 2);

	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
		{
			Effects->Shake = GL_FALSE;
		}
	}

	if (Ball->Position[1] >= this->Height)
	{
		this->ResetLevel();
		this->ResetPlayer();
	}

}



void Game::ProcessInput( GLfloat dt)
{

	if (this->State == GAME_ACTIVE)
	{
		GLfloat Velocity = PLAYER_VELOCITY * dt;
		//Move playerboard
		if (this->Keys['A'])
		{
			if (Player->Position[0] > 0)
			{
				Player->Position[0] -= Velocity;
				if (Ball->Stuck)
				{
					Ball->Position[0] -= Velocity;
				}
			}
			Keys['A'] = 0;
		}

		if (this->Keys['D'])
		{
			if (Player->Position[0] <= this->Width - Player->Size[0])
			{
				Player->Position[0] += Velocity;
				if (Ball->Stuck)
				{
					Ball->Position[0] += Velocity;
				}
				Keys['D'] = 0;
			}
		}

		if (this->Keys[' '])
		{
			Ball->Stuck = false;
			this->Keys[' '] = 0;
		}

		if (this->Keys['1'])
		{
			Effects->Chaos = GL_TRUE;
			Effects->Confuse = GL_FALSE;
			Effects->Shake = GL_FALSE;

			this->Keys['1'] = 0;
		}

		if (this->Keys['2'])
		{
			Effects->Chaos = GL_FALSE;
			Effects->Confuse = GL_TRUE;
			Effects->Shake = GL_FALSE;

			this->Keys['2'] = 0;
		}

		if (this->Keys['3'])
		{
			Effects->Chaos = GL_FALSE;
			Effects->Confuse = GL_FALSE;
			Effects->Shake = GL_TRUE;

			this->Keys['3'] = 0;
		}

		if (this->Keys['4'])
		{
			Effects->Chaos = GL_FALSE;
			Effects->Confuse = GL_FALSE;
			Effects->Shake = GL_FALSE;

			this->Keys['4'] = 0;
		}


	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		Effects->BeginRender();

		//DrawBackGround
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),vec2(0,0),vec2(this->Width,this->Height));
		//Draw level
		this->Levels[this->Level].Draw(*Renderer);
		//Draw player
		Player->Draw(*Renderer);
		//Draw particles
		Particles->Draw();
		//Draw renderer
		Ball->Draw(*Renderer);

		Effects->EndRender();

		Effects->Render(timer);
	}
	
}



/*
//center point of circle
vec2 center(one.Position[0] + one.radius);
//calculate AABB(axis aligned bounding box) info
vec2 aabb_half_extends(two.Size[0] / 2, two.Size[1] / 2);
vec2 aabb_center(two.Position[0] + aabb_half_extends[0], two.Position[1] + aabb_half_extends[1]);

//Get difference vector between both centers
vec2 difference = center - aabb_center;
vec2 clamped = vmath::clamp(difference, -aabb_half_extends, aabb_half_extends);
//add clamped value to AABB center and we get the value of the box closest to the circle
vec2 closest = aabb_center + clamped;
//Retrieve vector between center circle and closest point AABB and check if length <= radius
difference = closest - center;

*/


Direction VectorDirection(vec2 target)
{
	vec2 compass[] = {
		vec2(0.0,1.0),	//up
		vec2(1.0,0.0),	//right
		vec2(0.0,-1.0),	//down
		vec2(-1.0,0.0)	//left
	};

	GLfloat max = 0.0f;
	GLuint best_match = -1;

	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dp = dot(normalize(target), compass[i]);
		if (dp > max)
		{
			max = dp;
			best_match = i;
		}
	}

	return (Direction)best_match;
}

Collision CheckCollision(BallObject &one, GameObject & two)		//AABB - AABB collision
{
	vec2 center(one.Position + one.radius);

	//calculate AABB(axis aligned bounding box) info
	vec2 aabb_half_extends(two.Size[0] / 2, two.Size[1] / 2);
	vec2 aabb_center(two.Position[0] + aabb_half_extends[0], two.Position[1] + aabb_half_extends[1]);

	//Get difference vector between both centers
	vec2 difference = center - aabb_center;
	//vec2 clamped = vmath::max(-length(aabb_half_extends), vmath::min(length(difference), length(aabb_half_extends)));
	vec2 clamped = vmath::clamp(difference, -aabb_half_extends, aabb_half_extends);

	//add clamped value to AABB center and we get the value of the box closest to the circle
	vec2 closest = aabb_center + clamped;

	//Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;

	if (vmath::length(difference) <= one.radius)
	{
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	}
	else
	{
		return std::make_tuple(GL_FALSE, UP, vec2(0,0));
	}
}


GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    bool collisionX = one.Position[0] + one.Size[0] >= two.Position[0] &&
        two.Position[0] + two.Size[0] >= one.Position[0];
    // Collision y-axis?
    bool collisionY = one.Position[1] + one.Size[1] >= two.Position[1] &&
        two.Position[1] + two.Size[1] >= one.Position[1];
    // Collision only if on both axes
	


	return collisionX && collisionY;
}  









void Game::DoCollisions()
{
	for (GameObject &box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if(std::get<0>(collision))	//if collision true
			{ 
				if (!box.IsSolid)
				{
					box.Destroyed = GL_TRUE;
				}
				else
				{
					//if not solid enable shake effect
					ShakeTime = 0.05f;
					Effects->Shake = GL_TRUE;
				}
					//Collision resolution
					Direction dir = std::get<1>(collision);
					vec2 diff_vector = std::get<2>(collision);
					if (dir == LEFT || dir == RIGHT)
					{
						Ball->Velocity[0] = -Ball->Velocity[0];		//reverse velocity on x axis
						GLfloat penetration = Ball->radius - std::abs(diff_vector[0]);
						if (dir == LEFT)
						{
							//remove penetration
							Ball->Position[0] += penetration;
						}
						else
						{
							Ball->Position[0] -= penetration;
						}

					}
					else
					{
						Ball->Velocity[1] = -Ball->Velocity[1];
						
						GLfloat penetration = Ball->radius - std::abs(diff_vector[1]);
						if(dir == UP)
						{ 
							Ball->Position[1] -= penetration;
						}
						else
						{
							Ball->Position[1] += penetration;
						}
				}
			}
		}
	}

	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		//Check were it hit the board , and change velocity based on where it hit the board
		GLfloat centerBoard = Player->Position[0] + Player->Size[0] / 2;
		GLfloat distance = (Ball->Position[0] + Ball->radius) - centerBoard;
		GLfloat percentage = distance / (Player->Size[0] / 2);
		//Then move accordingly
		GLfloat strength = 2.0f;
		vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity[0] = INITIAL_BALL_VELOCITY[0] * percentage * strength;
		Ball->Velocity[1] = -1 * abs(Ball->Velocity[1]);
		Ball->Velocity = normalize(Ball->Velocity) * vmath::length(oldVelocity);
	}
}

