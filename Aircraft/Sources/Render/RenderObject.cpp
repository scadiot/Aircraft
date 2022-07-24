#include "RenderObject.h"

RenderObject::RenderObject(VertexArray* pVertexArray, Texture* pTexture, const glm::mat4& pTransform)
{
	mVertexArray = pVertexArray;
	mTexture = pTexture;
	mTransform = pTransform;
}

void RenderObject::setTexture(Texture* pTexture)
{
	mTexture = pTexture;
}

void RenderObject::setTransform(const glm::mat4& pTransform)
{
	mTransform = pTransform;
}

VertexArray* RenderObject::vertexArray()
{
	return mVertexArray;
}

Texture* RenderObject::texture()
{
	return mTexture;
}

glm::mat4 RenderObject::transform()
{
	return mTransform;
}