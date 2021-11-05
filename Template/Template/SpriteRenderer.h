#pragma once
#include "shader.h"
#include "Texture2D.h"

enum
{
	lPositionIndex = 0,
	lNormalIndex,
	lTexture0Index,
};


class SpriteRenderer
{
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D &texture, vmath::vec2 position, vmath::vec2 size = vmath::vec2(10, 10), GLfloat rotate = 0.0f, vmath::vec3 color = vmath::vec3(1.0f));
private:
	Shader shader;
	GLuint quadVAO;

	void initRenderData();
};
