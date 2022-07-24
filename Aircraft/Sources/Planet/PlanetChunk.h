#pragma once
#include <stdint.h>

class RenderController;

namespace Planet {
	class PlanetChunk {
	public:
		PlanetChunk(RenderController* pRenderController, uint32_t pZoom, uint32_t pX, uint32_t pY);
		void initialize();
		unsigned int getGridIndex(int pGridCount, int x, int y);
	protected:
		RenderController* mRenderController;
		uint32_t mZoom;
		uint32_t mX;
		uint32_t mY;
	};
}