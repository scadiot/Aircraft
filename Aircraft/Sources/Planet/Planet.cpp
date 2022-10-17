#include "planet.h"
#include "../Render/RenderController.h"
#include "PlanetChunk.h"
#include "ElevationController.h"
#include "../Render/TextureController.h"

const double Space::Planet::LatitudeMax = 85.0511287798066;
const double Space::Planet::Radius = 6371000;

Space::Planet::Planet(RenderController* pRenderController)
{
	mRenderController = pRenderController;

	mRenderController->setPlanetRadius(Space::Planet::Radius);
	mRenderController->setAtmosphereRadius(Space::Planet::Radius * 2);
	mElevationController = new ElevationController();
	mElevationController->loadFile("C:\\TMP\\aircraft\\tiles\\16.height");
	mElevationController->loadFile("C:\\TMP\\aircraft\\tiles\\15.height");
	mElevationController->loadFile("C:\\TMP\\aircraft\\tiles\\14.height");
	mElevationController->loadFile("C:\\TMP\\aircraft\\tiles\\13.height");
	mElevationController->loadFile("C:\\TMP\\aircraft\\tiles\\12.height");
	mElevationController->loadFile("C:\\TMP\\aircraft\\tiles\\11.height");
	mElevationController->loadFile("C:\\TMP\\aircraft\\tiles\\10.height");
	mElevationController->loadFile("C:\\TMP\\aircraft\\tiles\\9.height");

	mEmptyTexture = mRenderController->textureController()->loadTexture("0_0_0.jpg");

	for (int i = 0; i < 500;i++) {
		auto chunk = new PlanetChunk(mRenderController, this);
		mUnsedChunks.push(chunk);
	}

	this->mMainChunk = new PlanetChunk(mRenderController, this);
	this->mMainChunk->initialize(0, 0, 0);
}

void Space::Planet::update(glm::dvec3 pCameraPosition)
{
	this->mVisibleChunkCount = 0;
	this->mRenderController->clearRenderObject();
	this->updateChunkChildren(pCameraPosition, this->mMainChunk);
	this->pushRenderObjects(this->mMainChunk);
}

void Space::Planet::updateChunkChildren(glm::dvec3 pCameraPosition, PlanetChunk* pParent)
{
	pParent->update();
	if (!pParent->haveChildren()) {
		uint32_t newZoom = pParent->getZoom() + 1;
		uint32_t startX = pParent->getX() * 2;
		uint32_t startY = pParent->getY() * 2;
		PlanetChunk* children[4];
		for (int i = 0; i < 4; i++) {
			uint32_t x = startX + (i % 2);
			uint32_t y = startY + (i / 2);
			auto chunk = getChunk();
			chunk->initialize(newZoom, x, y);
			children[i] = chunk;
		}
		pParent->setChildren(children[0], children[1], children[2], children[3]);
	}

	bool allChidrenTexturesAreLoaded = false;
	
	for (int i = 0; i < 4;i++) {
		auto chunk = pParent->getChild(i);
		double distance = glm::distance(chunk->getBBoxCenter(), pCameraPosition);
		if (chunk->getZoom() == 16 || distance > chunk->getSize()) {
			this->trashChildrenChunk(chunk);
			chunk->clearChildren();
		}
		else {
			this->updateChunkChildren(pCameraPosition, chunk);
		}
	}
}

void Space::Planet::pushRenderObjects(PlanetChunk* pParent)
{
	int chunksToRenderCount = 0;
	if (pParent->haveChildren()) {
		for (int i = 0; i < 4; i++) {
			if (pParent->getChild(i)->allTexturesLoaded()) {
				chunksToRenderCount++;
			}
		}
		if (chunksToRenderCount == 4) {
			for (int i = 0; i < 4; i++) {
				this->pushRenderObjects(pParent->getChild(i));
			}
		}
		else {
			mRenderController->pushRenderObject(pParent->getRenderObject());
		}
	} else {
		mRenderController->pushRenderObject(pParent->getRenderObject());
	}
	//printf("test %i\r\n", pParent->getZoom());
}

Space::PlanetChunk* Space::Planet::getChunk()
{
	if (mUnsedChunks.empty()) {
		return new PlanetChunk(mRenderController, this);
	}

	auto chunk = mUnsedChunks.top();
	mUnsedChunks.pop();
	return chunk;
}

void Space::Planet::trashChunk(PlanetChunk* pChunk)
{
	mUnsedChunks.push(pChunk);
	trashChildrenChunk(pChunk);
}

void Space::Planet::trashChildrenChunk(PlanetChunk* pChunk) {
	if (!pChunk->haveChildren()) {
		return;
	}
	for (int i = 0; i < 4; i++) {
		auto child = pChunk->getChild(i);
		this->trashChunk(child);
	}
}

Texture* Space::Planet::getEmptyTexture()
{
	return mEmptyTexture;
}

Space::ElevationController* Space::Planet::getElevationController()
{
	return mElevationController;
}