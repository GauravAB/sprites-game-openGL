#pragma once
#include <map>
#include <string>
#include "Texture2D.h"
#include "shader.h"
#include "SOIL\SOIL.h"

class ResourceManager 
{
public:
	//resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	
	//load and generate a shader program 
	static Shader LoadShader(const GLchar*vs, const GLchar* tc, const  GLchar* te, const  GLchar* gs, const  GLchar*fs,std::string name);
	
	//retrieve the shader program
	static Shader GetShader(std::string name);

	//load and generate texture from a resource
	static Texture2D LoadTexture(const char* file, GLboolean alpha, std::string name);

	//retrieve a stored texture
	static Texture2D GetTexture(std::string name);

	//properly de-allocates all loaded resources
	static void ClearAllResources();


private:
	//no object just use member functions 
	ResourceManager() {}

	//loads and generates shader program
	static Shader loadShaderFromFile(const GLchar * vs, const GLchar* tc , const GLchar* te , const GLchar* gs, const GLchar* fs , const char* name);
	
	//loads and generates textures
	static Texture2D loadTextureFromResource(const char* file, GLboolean alpha);
	
	//file reader method
	static char* readTextFile(const GLchar* fileName);
};
