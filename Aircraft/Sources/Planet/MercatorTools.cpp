#include "MercatorTools.h"

#include <math.h>
#include <cmath>

Planet::TileCoord Planet::MercatorTools::GetTileCoordinates(uint32_t pZoom, Planet::PlanetCoord pPosition)
{
    double latRad = pPosition.latitude * M_PI / 180;
    double n = pow(2, pZoom);
    return Planet::TileCoord
    {
        (uint32_t)(n * ((pPosition.longitude + 180) / 360)),
        (uint32_t)(n * (1 - (log(tan(latRad) + 1 / cos(latRad)) / M_PI)) / 2)
    };
}

Planet::PlanetCoord Planet::MercatorTools::mercatorPositionToPlanetCoordinate(Planet::Position position)
{
    return Planet::PlanetCoord
    {
        position.x * 180,
        (180.0 / M_PI * (2.0 * atan(exp(position.y * M_PI)) - M_PI / 2)) * -1.0
    };
}

glm::dvec3  Planet::MercatorTools::mapTo3D(Planet::PlanetCoord pPosition)
{
    double radius = 5;
    double x = (radius * cos(toRadians(pPosition.latitude)) * cos(toRadians(pPosition.longitude)));
    double y = (radius * sin(toRadians(pPosition.latitude)));
    double z = (radius * cos(toRadians(pPosition.latitude)) * sin(toRadians(pPosition.longitude))) * -1.0f;

    return glm::dvec3{ x, y, z };
}

double Planet::MercatorTools::toRadians(double degree)
{
    return (degree * (M_PI / 180));
}