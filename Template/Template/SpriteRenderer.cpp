#include "SpriteRenderer.h"



SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer :: initRenderData()
{
	//Configure VAO , VBO
	GLuint VBO;
	GLfloat vertices[] =
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(this->quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(lPositionIndex, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
	glEnableVertexAttribArray(lPositionIndex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, vmath::vec2 position, vmath::vec2 size , GLfloat rotate , vmath::vec3 color)
{
	//prepare transformations
	this->shader.use();

	vmath::mat4 model_matrix;
	vmath::mat4 view_matrix;
	
	view_matrix = vmath::mat4::identity();
	model_matrix = vmath::mat4::identity();
	
	model_matrix = vmath::translate(position[0],position[1],0.0f);
	model_matrix *= vmath::translate(0.5f*size[0], 0.5f*size[1], 0.0f);
	model_matrix *= vmath::rotate(rotate, 0.0f, 0.0f, 1.0f);
	model_matrix *= vmath::translate(-0.5f*size[0], -0.5f*size[1], 0.0f);
	model_matrix *= vmath::scale(size[0], size[1], 0.0f);


	this->shader.setMat4("u_model_matrix",model_matrix,GL_FALSE);
	this->shader.setMat4("u_view_matrix", view_matrix, GL_FALSE);

	this->shader.setVec3("spriteColor", color);
	
	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}



