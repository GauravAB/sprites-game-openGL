#version 450 core

layout(location = 0) in vec4 vPosition;

out vec2 fTexCoords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main(void)
{
	gl_Position = vec4(vPosition.xy, 0.0, 1.0);
	
	vec2 texture = vPosition.zw;

	if (chaos)
	{
		float strength = 0.3;
		vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);
		fTexCoords = pos;
	}
	else if (confuse)
	{
		fTexCoords = vec2(1.0 - texture.x, 1.0 - texture.y);
	}
	else
	{
		fTexCoords = texture;
	}
	
	if (shake)
	{
		float strength = 0.01;
		gl_Position.x += cos(time * 10) * strength;
		gl_Position.y += cos(time * 15) * strength;
	}
}






