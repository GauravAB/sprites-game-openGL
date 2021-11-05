
#include "ResourceManager.h"
#include "Sysfiles.h"

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;




Texture2D ResourceManager::LoadTexture(const char* file, GLboolean alpha, std::string name)
{

	Textures[name] = loadTextureFromResource(file, alpha);

	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}


Texture2D ResourceManager::loadTextureFromResource(const char* file, GLboolean alpha)
{
	FILE *fp;
	Texture2D texture;
	
	fp = fopen("ResourceManager:LoadTextureErrorLog.txt", "a");

	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}

	//load image
	int width;
	int height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	//generate image
	texture.GenerateTexture(width,height,image);

	fclose(fp);
	return texture;
}


Shader ResourceManager::LoadShader(const GLchar * vs, const GLchar *tc, const GLchar *te, const GLchar *gs, const GLchar* fs,std::string name)
{
	const char* str = name.c_str();
	Shaders[name] = loadShaderFromFile(vs, tc, te, gs, fs ,str);

	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vs, const GLchar* tc, const GLchar* te, const GLchar* gs, const GLchar* fs , const char* name)
{
	const GLchar* vtext = NULL;
	const GLchar* tctext = NULL;
	const GLchar* tetext = NULL;
	const GLchar* gtext = NULL;
	const GLchar* ftext = NULL;

	if (vs)
	{
		vtext = readTextFile(vs);
	}
	if (tc)
	{
		tctext = readTextFile(tc);
	}
	if (te)
	{
		tetext = readTextFile(te);
	}
	if (gs)
	{
		gtext = readTextFile(gs);
	}
	if (fs)
	{
		ftext = readTextFile(fs);
	}

	Shader shader;
	shader.CompileShaderAndCreateProgram(vtext, tctext, tetext, gtext, ftext , name);

	return shader;
}


char* ResourceManager::readTextFile(const GLchar* fileName)
{
	FILE *fp , *ftex;
	if (fileName)
	{
		 fp = fopen(fileName, "rb");
		 ftex = fopen("ResourceManager ReadShaderFileErrorLog.txt", "w");

		if (fp == NULL)
		{
			fprintf_s(ftex, "ResourceManager::readTextFile : Cannot find Shader Source file path.\n");		
			fclose(ftex);
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}


	char* content = NULL;
	long numVal;

	//go till end
	fseek(fp, 0L, SEEK_END);

	//get count bytes 
	numVal = ftell(fp);

	//reset
	fseek(fp, 0L, SEEK_SET);

	content = (char*)malloc(sizeof(char)*(numVal + 1));

	//read into buffer
	fread(content, 1, numVal, fp);
	content[numVal] = '\0';

	fprintf_s(ftex, "ResourceManager::readTextFile : Shader Source file read Success.\n");

	fclose(fp);
	fclose(ftex);
	return content;
}



void ResourceManager::ClearAllResources()
{
	
	for (auto iter : Textures)
	{
		glDeleteTextures(1, &iter.second.ID);
	}
	for (auto iter : Shaders)
	{
		glDeleteProgram(iter.second.shaderProgramObjectID);
	}
}

