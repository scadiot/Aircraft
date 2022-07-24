#include "planet.h"
#include "../Render/RenderController.h"
#include "PlanetChunk.h"

Planet::Planet::Planet(RenderController* pRenderController)
{
	mRenderController = pRenderController;

	mChunks.push_back(new PlanetChunk(mRenderController, 1, 0, 0));
	mChunks.push_back(new PlanetChunk(mRenderController, 1, 1, 0));
	mChunks.push_back(new PlanetChunk(mRenderController, 1, 0, 1));
	mChunks.push_back(new PlanetChunk(mRenderController, 1, 1, 1));
}