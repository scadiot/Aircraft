#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera {
public:
	Camera();

	void setPosition(glm::dvec3 pPosition);
	void setDirection(glm::dvec3 pDirection);

	glm::dvec3 getPosition();
	glm::dvec3 getDirection();

	glm::dmat4 getProjectionMatrix();
protected:

	glm::dvec3 mPosition;
	glm::dvec3 mDirection;

	glm::dmat4 mProjection;
};