#include "Camera.h"

#include <glm/gtx/transform.hpp>


Camera::Camera()
{
	mProjection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 100.0f, 6371000.0f * 2.1f);
}

void Camera::setPosition(glm::dvec3 pPosition)
{
	mPosition = pPosition;
}

void Camera::setDirection(glm::dvec3 pDirection)
{
	mDirection = pDirection;
}

glm::dvec3 Camera::getPosition()
{
	return mPosition;
}

glm::dvec3 Camera::getDirection()
{
	return mDirection;
}

glm::dmat4 Camera::getProjectionMatrix()
{
	return mProjection;
}

