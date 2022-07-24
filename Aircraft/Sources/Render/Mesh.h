#pragma once

#include <GL/glew.h>

class aiMesh;
class VertexArray;
class Texture;

class Mesh {
public:
	Mesh(const aiMesh* mesh);
	VertexArray* vertexArray();

	void setTexture(Texture* pTexture);
protected:
	VertexArray* mVertexArray;
	Texture* mTexture;
};