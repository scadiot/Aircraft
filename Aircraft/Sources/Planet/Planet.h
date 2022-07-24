#pragma once
#include <vector>

class RenderController;

namespace Planet {
	class PlanetChunk;

	class Planet {
	public:
		Planet(RenderController* pRenderController);

		const double LatitudeMax = 85.0511287798066;
	protected:
		RenderController* mRenderController;
		std::vector<PlanetChunk*> mChunks;
	};
}
