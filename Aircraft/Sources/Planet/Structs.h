#pragma once

#include <stdint.h>

namespace Space {
	struct PlanetCoord {
		double latitude;
		double longitude;
	};

	struct Position {
		double x;
		double y;
	};

	struct TileCoord {
		uint32_t x;
		uint32_t y;
	};

	struct Position3D {
		double x;
		double y;
		double z;
	};
}