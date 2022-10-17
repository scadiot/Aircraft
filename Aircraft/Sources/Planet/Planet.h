#pragma once
#include <vector>
#include <stack>
#include <glm/glm.hpp>

class RenderController;
class Texture;

namespace Space {
	class PlanetChunk;
	class ElevationController;

	class Planet {
	public:
		Planet(RenderController* pRenderController);
		void update(glm::dvec3 pCameraPosition);
		void updateChunkChildren(glm::dvec3 pCameraPosition, PlanetChunk* pParent);
		void pushRenderObjects(PlanetChunk* pParent);
		Texture* getEmptyTexture();
		ElevationController* getElevationController();

		const static double LatitudeMax;
		const static double Radius;
	protected:
		PlanetChunk* getChunk();
		void trashChunk(PlanetChunk* pChunk);
		void trashChildrenChunk(PlanetChunk* pChunk);

		RenderController* mRenderController;
		ElevationController* mElevationController;
		std::vector<PlanetChunk*> mChunks;
		PlanetChunk* mMainChunk;
		uint32_t mVisibleChunkCount;
		std::stack<PlanetChunk*> mUnsedChunks;
		Texture* mEmptyTexture;
	};
}
