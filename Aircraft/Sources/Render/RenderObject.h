#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class VertexArray;
class Texture;

class RenderObject {
public:
	RenderObject();
	void initialize(VertexArray* pVertexArray, Texture* pTexture, const glm::dmat4& pTransform);
	void setTexture(Texture* pTexture);
	void setTransform(const glm::dmat4& pTransform);

	VertexArray* vertexArray();
	Texture* texture();
	glm::dmat4 transform();
protected:
	VertexArray* mVertexArray;
	Texture* mTexture;
	glm::dmat4 mTransform;
};