#include "PlanetChunk.h"
#include "MercatorTools.h"
#include "../Render/RenderController.h"
#include "../Render/VertexArray.h"
#include "../Render/RenderObject.h"
#include "../Render/Texture.h"
#include "../Render/TextureController.h"

#include <cmath>
#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

Planet::PlanetChunk::PlanetChunk(RenderController* pRenderController, uint32_t pZoom, uint32_t pX, uint32_t pY)
{
	mRenderController = pRenderController;
	mZoom = pZoom;
	mX = pX;
	mY = pY;
	initialize();
}

void Planet::PlanetChunk::initialize()
{
	double divisor = mZoom == 0 ? 1 : pow(2, mZoom);

	uint32_t gridCount = std::min(16u, 8 * (1 + (mZoom / 3)));
	uint32_t trianglesCount = gridCount * gridCount * 2;
	uint32_t verticesCount = trianglesCount * 3;


	double mercatorSize = 2.0 / divisor;
	double mercatorStep = mercatorSize / gridCount;
	double mercatorXStart = mercatorSize * mX - 1;
	double mercatorYStart = mercatorSize * mY - 1;

	PlanetCoord firstPosition = MercatorTools::mercatorPositionToPlanetCoordinate({ mercatorXStart, mercatorYStart });
	PlanetCoord lastPosition = MercatorTools::mercatorPositionToPlanetCoordinate({ mercatorXStart + mercatorSize, mercatorYStart + mercatorSize });
	glm::dvec3 chunkTranslation = MercatorTools::mapTo3D(firstPosition);

	int index = 0;
	std::vector<glm::dvec3> spacePositions;
	std::vector<VertexNormalUv> vertices;

	for (int indexLat = 0; indexLat < gridCount + 1; indexLat++)
	{
		for (int indexLon = 0; indexLon < gridCount + 1; indexLon++)
		{
			Position mercatorPosition = {
				mercatorXStart + mercatorStep * (double)indexLon,
				mercatorYStart + mercatorStep * (double)indexLat
			};

			PlanetCoord planetCoordinate = MercatorTools::mercatorPositionToPlanetCoordinate(mercatorPosition);
			glm::dvec3 spacePosition = MercatorTools::mapTo3D(planetCoordinate);
			spacePositions.push_back(spacePosition);

			VertexNormalUv vertex;
			vertex.mPosition[0] = spacePosition.x - chunkTranslation.x;
			vertex.mPosition[1] = spacePosition.y - chunkTranslation.y;
			vertex.mPosition[2] = spacePosition.z - chunkTranslation.z;
			glm::dvec3 normal = glm::normalize(spacePosition);
			vertex.mNormal[0] = normal.x;
			vertex.mNormal[1] = normal.y;
			vertex.mNormal[2] = normal.z;

			vertex.mUv[0] = (float)indexLon / (gridCount + 1);
			vertex.mUv[1] = (float)indexLat / (gridCount + 1);
			vertices.push_back(vertex);
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

	Texture* texture = mRenderController->textureController()->loadTexture("earth\\Textures\\earth.png");

	VertexArray* vertexArray = new VertexArray(&vertices[0], vertices.size());
	vertexArray->setIndices(&indices[0], indices.size());
	RenderObject* renderObject = new RenderObject(vertexArray, texture, glm::mat4(1));

	mRenderController->pushRenderObject(renderObject);
}

unsigned int Planet::PlanetChunk::getGridIndex(int pGridCount, int x, int y) {
	return y * (pGridCount + 1) + x;
}