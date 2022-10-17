#pragma once

#include "Structs.h"
#include <glm/glm.hpp>

namespace Space {
	class MercatorTools {
	public:
		static TileCoord GetTileCoordinates(uint32_t pZoom, PlanetCoord pPosition);
		static PlanetCoord mercatorPositionToPlanetCoordinate(Space::Position pPosition);
		static glm::dvec3 mapTo3D(PlanetCoord pPosition, double pElevation);

		static double toRadians(double degree);
	};
}
