#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class VertexArray;
class Texture;

class RenderObject {
public:
	RenderObject(VertexArray* pVertexArray, Texture* pTexture, const glm::mat4& pTransform);

	void setTexture(Texture* pTexture);
	void setTransform(const glm::mat4& pTransform);

	VertexArray* vertexArray();
	Texture* texture();
	glm::mat4 transform();
protected:
	VertexArray* mVertexArray;
	Texture* mTexture;
	glm::mat4 mTransform;
};