#version 450 core

in vec2 fTexCoords;
in vec4 ParticleColor;
out vec4 FragColor;

uniform sampler2D sprite;

void main(void)
{
	FragColor = (texture(sprite,fTexCoords) * ParticleColor);
}



