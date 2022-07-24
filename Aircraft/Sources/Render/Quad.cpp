#include "Quad.h"
#include "VertexArray.h"

VertexNormalUv vertices[] = {
    { {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
    { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

Quad::Quad()
{
    mVertexArray = new VertexArray(vertices, 4);
    mVertexArray->setIndices(indices, 6);
}

VertexArray* Quad::vertexArray()
{
	return mVertexArray;
}