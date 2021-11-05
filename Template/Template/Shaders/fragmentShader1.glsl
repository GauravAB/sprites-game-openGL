#version 450 core

out vec4 FragColor;
uniform vec2 u_resolution;

void main(void)
{
	vec2 uv = gl_FragCoord.xy / u_resolution;

	FragColor = vec4(1.0);
}


