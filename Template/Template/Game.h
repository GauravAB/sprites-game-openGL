#pragma once
#include "glFiles.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"



enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, vec2> Collision;

class Game
{
public:
	GameState State;
	GLboolean Keys[1024];
	GLuint Width, Height;
	std::vector <GameLevel> Levels;
	GLuint Level;

	Game(GLuint Width, GLuint Height);
	~Game();
	
	//Initialize game state (load all shaders / textures / levels)
	void Initialize();
	void Uninitialize();

	//game loop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	void DoCollisions();
	void Game::ResetLevel();
	void Game::ResetPlayer();
};

