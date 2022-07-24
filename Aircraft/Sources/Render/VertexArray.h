#pragma once

#include <string>
#include <GL/glew.h>

class RenderController;

struct VertexNormalUv
{
	float mPosition[3];
	float mNormal[3];
	float mUv[2];
};

class VertexArray {
public:
	VertexArray(const VertexNormalUv* vertices, unsigned int pVerticesCount);
	void setIndices(const unsigned int* indices, unsigned int pIndicesCount);
	GLuint id();
	void bind();
	GLuint facesCount();
protected:
	RenderController* mRenderController;
	GLuint mVertexArrayId;
	GLuint mFacesCount;
};