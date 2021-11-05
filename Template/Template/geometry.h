#pragma once
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265358979323846

typedef struct mesh
{
	float *verts;
	float *norms;
	float *texCoords;
	unsigned int *elements;
	unsigned int numVertices;
	unsigned int numElements;
	unsigned int maxElements;
};

struct Vertex
{
	float position[3];
	float normal[3];
	float texture[3];

	Vertex() {};
	Vertex(float x, float y, float z, float n1, float n2, float n3, float t1, float t2, float scale)
	{
		position[0] = scale * x; position[1] = scale * y; position[2] = scale * z;
		normal[0] = n1; normal[1] = n2; normal[2] = n3;
		texture[0] = t1; texture[1] = t2;
	}
};

void createCube(float size, Vertex *cubeVertex);
void makeSphere(mesh *sphere, int slicesCount, int stacksCount, float radius);

