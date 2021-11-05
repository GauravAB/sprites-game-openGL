#version 450 core

out vec4 FragColor;

in vec2 fTexCoords;
uniform sampler2D image;
uniform vec3 spriteColor;

void main(void)
{
	FragColor =  vec4(spriteColor,1.0) * texture(image,fTexCoords);
}








