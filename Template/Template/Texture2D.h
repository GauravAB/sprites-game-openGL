#pragma once
#include "Sysfiles.h"
#include "glFiles.h"


class Texture2D
{
public :
	//hold the ID for the texture 
	GLuint ID;
	//hold texture width and height
	GLuint  Width , Height;
	//texture format
	GLuint Internal_Format;	//format to texture image
	GLuint Image_Format;	//format to loaded image
	
	//texture configuration
	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Max;

	//Constructor
	Texture2D();
	//Generate texture from image
	void GenerateTexture(GLuint width, GLuint height, unsigned char* data);
	//Bind the texture as current active texture
	void Bind() const;
};


