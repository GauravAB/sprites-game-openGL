#include "GameLevel.h"

void GameLevel::Draw(SpriteRenderer &renderer)
{
	//c++11 syntax for iteration similar too
	//i = 0 ; i < Bricks.size() ; i++
		//GameObject &tile = this->Bricks[i]
	for (GameObject &tile : this->Bricks)
	{
		if (!tile.Destroyed)
		{
			tile.Draw(renderer);
		}
	}
}

GLboolean GameLevel::IsCompleted()
{
	for (GameObject &tile : this->Bricks)
	{
		if (!tile.IsSolid && !tile.Destroyed)
		{
			return GL_FALSE;
		}
	}
	return GL_TRUE;
}


void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
	//Clear old data
	this->Bricks.clear();
	//Load from file
	GLuint tileCode;
	GameLevel level;

	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;

	if (fstream)
	{
		while (std::getline(fstream, line))	//read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)		//read each word separated by spaces
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
		{
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}


void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint lvlWidth, GLuint lvlHeight)
{
	//Calculate dimensions
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = lvlWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = lvlHeight / height;

	//Initialize level tiles based on tiledata
	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			//check block type from 2D level array
			if (tileData[y][x] == 1) //solid block
			{
				vec2 pos( unit_width*x, unit_height*y);
				vec2 size(unit_width, unit_height);
				
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = GL_TRUE;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) //
			{
				vec3 color = vec3(1.0f);	//original::white
				if (tileData[y][x] == 2)
				{
					color = vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tileData[y][x] == 3)
				{
					color = vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tileData[y][x] == 4)
				{
					color = vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tileData[y][x] == 5)
				{
					color = vec3(1.0f, 0.5f, 0.0f);
				}

				vec2 pos(unit_width *x, unit_height * y);
				vec2 size(unit_width, unit_height);
				this->Bricks.push_back(
					GameObject(pos, size, ResourceManager::GetTexture("block"), color));

			}
		}
	}
}





