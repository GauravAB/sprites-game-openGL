#pragma once
#include <vector>
#include "GameObject.h"
#include <string>
#include <sstream>
#include "ResourceManager.h"


class GameLevel
{
public:
	std::vector<GameObject>Bricks;

	GameLevel() {}
	//Loads level from external file
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	//Render level
	void Draw(SpriteRenderer &renderer);
	//check if the level is completed (all non solid tiles will get destroyed)
	GLboolean IsCompleted();
private:
	//initialize level from tile data
	void init(std::vector<std::vector<GLuint>>tileData, GLuint levelWidth, GLuint levelHeight);
};

