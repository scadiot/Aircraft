#pragma once
#include "Structs.h"
#include <glm/glm.hpp>

namespace Planet {
	class MercatorTools {
	public:
		static TileCoord GetTileCoordinates(uint32_t pZoom, PlanetCoord pPosition);
		static PlanetCoord mercatorPositionToPlanetCoordinate(Planet::Position position);
		static glm::dvec3 mapTo3D(PlanetCoord pPosition);

		static double toRadians(double degree);
	};
}
