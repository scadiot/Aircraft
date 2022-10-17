#include "PlanetChunk.h"
#include "Planet.h"
#include "MercatorTools.h"
#include "../Render/RenderController.h"
#include "../Render/VertexArray.h"
#include "../Render/RenderObject.h"
#include "../Render/Texture.h"
#include "../Render/TextureController.h"
#include "ElevationController.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>

uint32_t Space::PlanetChunk::GridCount = 16u;

Space::PlanetChunk::PlanetChunk(RenderController* pRenderController, Planet* pPlanet)
{
	mRenderController = pRenderController;
	mPlanet = pPlanet;
	mVertexArray = new VertexArray();
	mRenderObject = new RenderObject();
	mTextureLoaded = false;
}

void Space::PlanetChunk::initialize(uint32_t pZoom, uint32_t pX, uint32_t pY)
{
	mZoom = pZoom;
	mX = pX;
	mY = pY;

	mVisible = false;
	mHaveChildren = false;

	double divisor = mZoom == 0 ? 1 : pow(2, mZoom);

	uint32_t gridCount = 16u;// std::min(16u, 8 * (1 + (mZoom / 3)));

	double mercatorSize = 2.0 / divisor;
	double mercatorStep = mercatorSize / gridCount;
	double mercatorXStart = mercatorSize * mX -1;
	double mercatorYStart = mercatorSize * mY -1;

	PlanetCoord firstPosition = MercatorTools::mercatorPositionToPlanetCoordinate({ mercatorXStart, mercatorYStart });
	glm::dvec3 chunkTranslation = MercatorTools::mapTo3D(firstPosition, 0);

	std::vector<glm::dvec3> spacePositions;
	std::vector<VertexNormalUv> vertices;

	float r = (float)(std::rand() % 10) / 10.0f;
	float g = (float)(std::rand() % 10) / 10.0f;
	float b = (float)(std::rand() % 10) / 10.0f;

	mBBoxMin = glm::dvec3(std::numeric_limits<double>::max());
	mBBoxMax = glm::dvec3(std::numeric_limits<double>::lowest());

	for (uint32_t indexLat = 0; indexLat < gridCount + 1; indexLat++)
	{
		for (uint32_t indexLon = 0; indexLon < gridCount + 1; indexLon++)
		{
			Position mercatorPosition = {
				mercatorXStart + mercatorStep * (double)indexLon,
				mercatorYStart + mercatorStep * (double)indexLat
			};
			
			PlanetCoord planetCoordinate = MercatorTools::mercatorPositionToPlanetCoordinate(mercatorPosition);
			double elevation = 0;
			elevation = mPlanet->getElevationController()->getElevation(mZoom, mX, mY, indexLon, indexLat);
			glm::dvec3 spacePosition = MercatorTools::mapTo3D(planetCoordinate, elevation);
			spacePositions.push_back(spacePosition);

			VertexNormalUv vertex;
			vertex.mPosition[0] = spacePosition.x - chunkTranslation.x;
			vertex.mPosition[1] = spacePosition.y - chunkTranslation.y;
			vertex.mPosition[2] = spacePosition.z - chunkTranslation.z;
			glm::dvec3 normal = glm::normalize(spacePosition);
			vertex.mNormal[0] = normal.x;
			vertex.mNormal[1] = normal.y;
			vertex.mNormal[2] = normal.z;

			vertex.mUv[0] = (float)indexLon / gridCount;
			vertex.mUv[1] = 1.0f - (float)indexLat / gridCount;
			vertices.push_back(vertex);

			mBBoxMin.x = std::min(mBBoxMin.x, spacePosition.x);
			mBBoxMin.y = std::min(mBBoxMin.y, spacePosition.y);
			mBBoxMin.z = std::min(mBBoxMin.z, spacePosition.z);
			mBBoxMax.x = std::max(mBBoxMax.x, spacePosition.x);
			mBBoxMax.y = std::max(mBBoxMax.y, spacePosition.y);
			mBBoxMax.z = std::max(mBBoxMax.z, spacePosition.z);
		}
	}

	std::vector<unsigned int> indices;
	for (int y = 0; y < gridCount; y++) {
		for (int x = 0; x < gridCount; x++) {
			indices.push_back(getGridIndex(gridCount, x, y));
			indices.push_back(getGridIndex(gridCount, x + 1, y));
			indices.push_back(getGridIndex(gridCount, x + 1, y + 1));

			indices.push_back(getGridIndex(gridCount, x, y));
			indices.push_back(getGridIndex(gridCount, x + 1, y + 1));
			indices.push_back(getGridIndex(gridCount, x, y + 1));
		}
	}

	mBBoxCenter.x = mBBoxMin.x + ((mBBoxMax.x - mBBoxMin.x) / 2.0);
	mBBoxCenter.y = mBBoxMin.y + ((mBBoxMax.y - mBBoxMin.y) / 2.0);
	mBBoxCenter.z = mBBoxMin.z + ((mBBoxMax.z - mBBoxMin.z) / 2.0);

	mSize = glm::distance(mBBoxMax, mBBoxMin);

	mTexture = mRenderController->textureController()->loadTexture(std::to_string(mZoom) + "\\" + std::to_string(mZoom) + "_" + std::to_string(mY) + "_" + std::to_string(mX)  + ".png");

	mVertexArray->setVertices(&vertices[0], vertices.size());
	mVertexArray->setIndices(&indices[0], indices.size());
	mRenderObject->initialize(mVertexArray, mTexture, glm::translate(glm::dmat4(1), chunkTranslation));
}

unsigned int Space::PlanetChunk::getGridIndex(int pGridCount, int x, int y)
{
	return y * (pGridCount + 1) + x;
}

glm::dvec3 Space::PlanetChunk::getBBoxCenter()
{
	return this->mBBoxCenter;
}

double Space::PlanetChunk::getSize()
{
	return this->mSize;
}

void Space::PlanetChunk::setVisible(bool pValue)
{
	this->mVisible = pValue;
}

RenderObject* Space::PlanetChunk::getRenderObject()
{
	return this->mRenderObject;
}

bool Space::PlanetChunk::haveChildren()
{
	return this->mHaveChildren;
}

void Space::PlanetChunk::setChildren(PlanetChunk* child1, PlanetChunk* child2, PlanetChunk* child3, PlanetChunk* child4)
{
	this->mHaveChildren = true;
	this->mChildrens[0] = child1;
	this->mChildrens[1] = child2;
	this->mChildrens[2] = child3;
	this->mChildrens[3] = child4;
}

Space::PlanetChunk* Space::PlanetChunk::getChild(uint32_t pIndex)
{
	return this->mChildrens[pIndex];
}

void Space::PlanetChunk::clearChildren()
{
	this->mHaveChildren = false;
}

uint32_t Space::PlanetChunk::getZoom()
{
	return this->mZoom;
}

uint32_t Space::PlanetChunk::getX()
{
	return this->mX;
}

uint32_t Space::PlanetChunk::getY()
{
	return this->mY;
}

void Space::PlanetChunk::update()
{
	if (mTextureLoaded && !mTexture->isLoading()) {
		mRenderObject->setTexture(mTexture);
		mTextureLoaded = false;
	}
}

bool Space::PlanetChunk::allTexturesLoaded()
{
	return mTexture->isLoading();
	//if (this->mHaveChildren) {
	//	return mTextureLoaded && 
	//		mChildrens[0]->allTexturesLoaded() && 
	//		mChildrens[1]->allTexturesLoaded() && 
	//		mChildrens[2]->allTexturesLoaded() &&
	//		mChildrens[3]->allTexturesLoaded();
	//}
	//else {
	//	return mTextureLoaded;
	//}
}