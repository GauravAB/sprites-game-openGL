#pragma once
#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "shader.h"

class PostProcessor
{
public:
	//State
	Shader PostProcessorShader;
	Texture2D Texture;
	GLuint Width, Height;
	//options
	GLboolean Confuse, Chaos, Shake;
	//Constructor
	PostProcessor(Shader shader, GLuint width, GLuint height);
	//prepare frameBuffer
	void BeginRender();
	//end framebuffer
	void EndRender();
	//render to texture display
	void Render(GLfloat time);
private:
	//Render state
	GLuint MSFBO, FBO;	//MSFBO = Multisampled FBO, blit MS color-buffer to texture
	GLuint RBO;
	GLuint VAO;
	//initialize quad for rendering postprocessing  technique
	void initRenderData();
};








