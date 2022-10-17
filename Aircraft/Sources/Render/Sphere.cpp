#include "Sphere.h"
#include "VertexArray.h"

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <cmath>

double toRadians(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}

glm::vec3 mapTo3D(double longitude, double latitude)
{
    double radius = 5;
    float x = (radius * cos(toRadians(latitude)) * cos(toRadians(longitude)));
    float y = (radius * sin(toRadians(latitude)));
    float z = (radius * cos(toRadians(latitude)) * sin(toRadians(longitude))) * -1.0f;

    return glm::vec3(x, y, z);
}

unsigned int Sphere::getGridIndex(int x, int y) {
    return y * (gridSize + 1) + x;
}

Sphere::Sphere()
{
    
    std::vector<VertexNormalUv> vertices;
    for (int y = 0; y < gridSize + 1;y++) {
        for (int x = 0; x < gridSize + 1; x++) {
            double lat = ((double)x / (double)gridSize) * 180.0 - 90.0;
            double lon = ((double)y / (double)gridSize) * 360.0 - 180.0;
            glm::vec3 position = mapTo3D(lon, lat);
            glm::vec3 normal = glm::normalize(position);
            VertexNormalUv vertex = {
                {position.x, position.y, position.z},
                {normal.x, normal.y, normal.z},
                {(float)x / gridSize, (float)y / gridSize}
            };
            vertices.push_back(vertex);
        }
    }
    
    std::vector<unsigned int> indices;
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            indices.push_back(getGridIndex(x, y));
            indices.push_back(getGridIndex(x + 1, y));
            indices.push_back(getGridIndex(x + 1, y + 1));

            indices.push_back(getGridIndex(x, y));
            indices.push_back(getGridIndex(x + 1, y + 1));
            indices.push_back(getGridIndex(x, y + 1));
        }
    }

    mVertexArray = new VertexArray();
    mVertexArray->setVertices(&vertices[0], vertices.size());
    mVertexArray->setIndices(&indices[0], indices.size());
}

VertexArray* Sphere::vertexArray()
{
    return mVertexArray;
}