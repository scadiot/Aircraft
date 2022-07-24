#pragma once

#include <GL/glew.h>

class VertexArray;

class Quad {
public:
	Quad();
	VertexArray* vertexArray();
protected:
	VertexArray* mVertexArray;
};