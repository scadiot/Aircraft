#include "ElevationController.h"
#include <fstream>

Space::ElevationController::ElevationController()
{

}

void Space::ElevationController::loadFile(std::string pPath)
{
	ElevationData* elevationData = new ElevationData(pPath);
	mElevationDatas[elevationData->getZoom()].push_back(elevationData);
}

double Space::ElevationController::getElevation(uint32_t pZoom, uint32_t pTileX, uint32_t pTileY, uint32_t offsetX, uint32_t offsetY)
{
	for (auto elevationData : mElevationDatas[pZoom]) {
		if (elevationData->contain(pTileX, pTileY)) {
			auto elevation = elevationData->getElevation(pTileX, pTileY, offsetX, offsetY);
			return (double)elevation;
		}
	}
	return 0;
}

Space::ElevationData::ElevationData(std::string pPath)
{
	std::ifstream file(pPath, std::ios::binary);
	file.read((char*)&mZoom, sizeof(uint32_t));
	file.read((char*)&mStartTileX, sizeof(uint32_t));
	file.read((char*)&mStartTileY, sizeof(uint32_t));
	file.read((char*)&mEndTileX, sizeof(uint32_t));
	file.read((char*)&mEndTileY, sizeof(uint32_t));
	file.read((char*)&mDataSize, sizeof(uint32_t));
	mData = new uint16_t[mDataSize];
	while (file) {
		file.read((char*)mData, mDataSize * 2);
	}
	//printf("%i\r\n", file.gcount());
	//do {
	//	int i = file.read((char*)(mData + readSize), sizeToRead - readSize).gcount();
	//	printf("%i\r\n", i);
	//} while (readSize < mDataSize * sizeof(uint16_t));
	//for (int i = 0; i < 300;i++) {
	//	printf("%i\r\n", mData[i]);
	//}
}

uint16_t Space::ElevationData::getElevation(uint32_t pTileX, uint32_t pTileY, uint32_t pOffsetX, uint32_t pOffsetY)
{
	uint32_t tileX = pTileX - mStartTileX;
	uint32_t tileY =  pTileY - mStartTileY;
	int countByLine = 16 * ((mEndTileY - mStartTileY) + 1);
	if ((tileX * 16) + pOffsetX >= countByLine || (tileY * 16) + pOffsetY >= countByLine) {
		return 0;
	}
	int value = mData[(((tileY * 16) + pOffsetY) * countByLine) + ((tileX * 16) + pOffsetX)];
	return value;
}

uint32_t Space::ElevationData::getZoom() {
	return mZoom;
}

bool Space::ElevationData::contain(uint32_t pTileX, uint32_t pTileY) {
	return (pTileX >= mStartTileX && pTileX <= mEndTileX) && (pTileY >= mStartTileY && pTileY <= mEndTileY);
}