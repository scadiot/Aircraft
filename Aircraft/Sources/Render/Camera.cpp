#include "Camera.h"

#include <glm/gtx/transform.hpp>


Camera::Camera()
{
	mUpdateViewNeeded = true;
	mProjection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);
}

void Camera::setPosition(glm::vec3 pPosition)
{
	mPosition = pPosition;
	mUpdateViewNeeded = true;
}

void Camera::setTarget(glm::vec3 pTarget)
{
	mTarget = pTarget;
	mUpdateViewNeeded = true;
}

glm::vec3 Camera::getPosition()
{
	return mPosition;
}

glm::vec3 Camera::getTarget()
{
	return mTarget;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return mProjection;
}

glm::mat4 Camera::getViewMatrix()
{
	if (mUpdateViewNeeded) {
		mView = glm::lookAt(mPosition, mTarget, glm::vec3(0, 1, 0));
		mUpdateViewNeeded = false;
	}
	return mView;
}
