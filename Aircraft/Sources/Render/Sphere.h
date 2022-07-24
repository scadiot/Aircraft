#pragma once

#include <GL/glew.h>

class VertexArray;

class Sphere {
public:
	Sphere();
	VertexArray* vertexArray();
protected:
	unsigned int getGridIndex(int x, int y);

	VertexArray* mVertexArray;
	const int gridSize = 50;
};