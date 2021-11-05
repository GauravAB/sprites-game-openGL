#include "PostProcessor.h"
#include <iostream>

PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
	:PostProcessorShader(shader), Texture(), Width(width), Height(height), Confuse(GL_FALSE), Chaos(GL_FALSE), Shake(GL_FALSE)
{
	FILE *fp;
	fp = fopen("PostProcessorLog.txt", "w");

	//Initialize renderbuffer/framebuffer object
	glGenFramebuffers(1, &this->MSFBO);
	glGenFramebuffers(1, &this->FBO);
	glGenRenderbuffers(1, &this->RBO);

	//initialize renderbuffer storage with a multi sampled color buffer (depth and stencil not required in this game)
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	//allocate storage for render buffer
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
	//attach multisample render buffer to the current frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(fp, "MSFBO failed to createn\n");
	}

	//also initialize the FBO/texture to blit multisampled color-buffer to; used for  shader operations
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	this->Texture.GenerateTexture(width, height, NULL);
	//attach texture to frambuffer as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(fp, "failed to initialize FBO\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Initialize render data and uniforms
	this->initRenderData();
	this->PostProcessorShader.setInt("scene", 0);
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
		{-offset,offset},	//top left
		{0.0f	,offset},	//top center
		{offset ,offset},	//top right
		{-offset,  0.0f},	//center left
		{0.0f,	   0.0f},		//center
		{offset,   0.0f},	//center right
		{-offset,-offset},	//bottom left
		{0.0f,	-offset},	//bottom center
		{offset ,-offset}  //bottom right
	};
	
	glUniform2fv(glGetUniformLocation(this->PostProcessorShader.shaderProgramObjectID, "offsets"), 9, (GLfloat*)offsets);
	GLint edge_kernel[9] = 
	{
		-1, -1, -1,
		-1, 8 , -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(this->PostProcessorShader.shaderProgramObjectID, "edge_kernel"), 9, edge_kernel);

	GLfloat blur_kernel[9] = 
	{
		1.0/16,2.0/16,1.0/16,
		2.0/16,4.0/16,2.0/16,
		1.0/16,2.0/16,1.0/16
	};
	glUniform1fv(glGetUniformLocation(this->PostProcessorShader.shaderProgramObjectID, "blur_kernel"), 9, blur_kernel);
	
	fprintf(fp, "log closed\n");
	fclose(fp);
}

void PostProcessor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
	//Now resolve multisampled color-buffer into intermediate FBO to store  the texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//Binds both READ and WRITE framebuffer to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(GLfloat time)
{
	//set uniform/options
	this->PostProcessorShader.use();
	this->PostProcessorShader.setFloat("time",time);
	this->PostProcessorShader.setInt("confuse",this->Confuse);
	this->PostProcessorShader.setInt("chaos",this->Chaos);
	this->PostProcessorShader.setInt("shake",this->Shake);

	//Render textured quad
	glActiveTexture(GL_TEXTURE0);
	this->Texture.Bind();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES,0,6);
	glBindVertexArray(0);
}


void PostProcessor::initRenderData()
{
	GLuint VBO;
	GLfloat vertices[] = 
	{
		//pos		//tex
		-1.0f,- 1.0f,0.0f,0.0f,
		 1.0f,  1.0f,1.0f,1.0f,
		-1.0f,  1.0f,0.0f,1.0f,

		-1.0f, -1.0f,0.0f,0.0f,
		 1.0f, -1.0f,1.0f,0.0f,
		 1.0f,  1.0f,1.0f,1.0f
	};
	
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}









