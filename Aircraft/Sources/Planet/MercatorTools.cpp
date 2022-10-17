#include "MercatorTools.h"
#include "Planet.h"

#include <math.h>
#include <cmath>

Space::TileCoord Space::MercatorTools::GetTileCoordinates(uint32_t pZoom, Space::PlanetCoord pPosition)
{
    double latRad = pPosition.latitude * M_PI / 180;
    double n = pow(2, pZoom);
    return Space::TileCoord
    {
        (uint32_t)(n * ((pPosition.longitude + 180) / 360)),
        (uint32_t)(n * (1 - (log(tan(latRad) + 1 / cos(latRad)) / M_PI)) / 2)
    };
}

Space::PlanetCoord Space::MercatorTools::mercatorPositionToPlanetCoordinate(Space::Position pPosition)
{
    return Space::PlanetCoord
    {
        (180.0 / M_PI * (2.0 * atan(exp(pPosition.y * M_PI)) - M_PI / 2)) * -1.0,
        pPosition.x * 180
    };
}

glm::dvec3  Space::MercatorTools::mapTo3D(Space::PlanetCoord pPosition, double pElevation)
{
    double x = ((Planet::Radius + pElevation) * cos(toRadians(pPosition.latitude)) * cos(toRadians(pPosition.longitude)));
    double y = ((Planet::Radius + pElevation) * sin(toRadians(pPosition.latitude)));
    double z = ((Planet::Radius + pElevation) * cos(toRadians(pPosition.latitude)) * sin(toRadians(pPosition.longitude))) * -1.0f;

    return glm::dvec3{ x, y, z };
}

double Space::MercatorTools::toRadians(double degree)
{
    return (degree * (M_PI / 180));
}