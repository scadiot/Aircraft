#pragma once
#include <string>
#include <vector>

namespace Space {
	class ElevationData {
	public:
		ElevationData(std::string pPath);
		uint16_t getElevation(uint32_t pTileX, uint32_t pTileY, uint32_t pOffsetX, uint32_t pOffsetY);
		uint32_t getZoom();
		bool contain(uint32_t pTileX, uint32_t pTileY);
	protected:
		uint32_t mZoom;
		uint32_t mStartTileX;
		uint32_t mStartTileY;
		uint32_t mEndTileX;
		uint32_t mEndTileY;
		uint32_t mDataSize;
		uint16_t* mData;
	};

	class ElevationController {
	public:
		ElevationController();
		void loadFile(std::string pPath);
		double getElevation(uint32_t pZoom, uint32_t pTileX, uint32_t pTileY, uint32_t offsetX, uint32_t offsetY);
	protected:
		std::vector<ElevationData*> mElevationDatas[20];
	};
}