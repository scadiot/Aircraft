#include "RenderObject.h"

RenderObject::RenderObject()
{

}

void RenderObject::initialize(VertexArray* pVertexArray, Texture* pTexture, const glm::dmat4& pTransform)
{
	mVertexArray = pVertexArray;
	mTexture = pTexture;
	mTransform = pTransform;
}

void RenderObject::setTexture(Texture* pTexture)
{
	mTexture = pTexture;
}

void RenderObject::setTransform(const glm::dmat4& pTransform)
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

glm::dmat4 RenderObject::transform()
{
	return mTransform;
}