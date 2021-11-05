#version 450 core

in vec2 fTexCoords;
out vec4 color;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;


void main(void)
{
	color = vec4(0.0f);
	vec3 samplebox[9];	

	//sample from texture offsets if using convolution matrix
	if ( chaos || shake)
	{
		for (int i = 0; i < 9; i++)
		{
			samplebox[i] = vec3(texture(scene, fTexCoords.st + offsets[i]));
		}
	}

	//process effects
	if (chaos)
	{
		for (int i = 0; i < 9; i++)
		{
			color += vec4(samplebox[i] * edge_kernel[i], 0.0f);
		}
		color.a = 1.0f;
	}
	else if (confuse)
	{
		color = vec4(1.0 - texture(scene, fTexCoords).rgb, 1.0);
	}
	else if (shake)
	{
		for (int i = 0; i < 9; i++)
		{
			color += vec4(samplebox[i] * blur_kernel[i], 0.0f);
		}
		color.a = 1.0f;

	}
	else
	{
		color = texture(scene, fTexCoords);
	}

}


