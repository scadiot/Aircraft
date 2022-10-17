#pragma once
#include <stdint.h>
#include <glm/glm.hpp>

class RenderController;
class RenderObject;
class VertexArray;
class Texture;

namespace Space {
	class Planet;
	class PlanetChunk {
	public:
		PlanetChunk(RenderController* pRenderController, Planet* pPlanet);
		void initialize(uint32_t pZoom, uint32_t pX, uint32_t pY);
		void loadElevation();
		unsigned int getGridIndex(int pGridCount, int x, int y);
		glm::dvec3 getBBoxCenter();
		double getSize();
		void setVisible(bool pValue);
		RenderObject* getRenderObject();

		bool haveChildren();
		void setChildren(PlanetChunk* child1, PlanetChunk* child2, PlanetChunk* child3, PlanetChunk* child4);
		PlanetChunk* getChild(uint32_t pIndex);
		void clearChildren();

		uint32_t getZoom();
		uint32_t getX();
		uint32_t getY();

		void update();

		bool allTexturesLoaded();

		static uint32_t GridCount;
	protected:
		RenderController* mRenderController;
		Space::Planet* mPlanet;
		uint32_t mZoom;
		uint32_t mX;
		uint32_t mY;
		glm::dvec3 mBBoxMin;
		glm::dvec3 mBBoxMax;
		glm::dvec3 mBBoxCenter;
		double mSize;
		bool mVisible;

		RenderObject* mRenderObject;
		Texture* mTexture;

		bool mHaveChildren;
		PlanetChunk* mChildrens[4];
		VertexArray* mVertexArray;
		bool mTextureLoaded;
	};
}