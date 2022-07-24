#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera {
public:
	Camera();

	void setPosition(glm::vec3 pPosition);
	void setTarget(glm::vec3 pTarget);

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void updateMAtrix();
protected:
	void updateMatrix();

	glm::vec3 mPosition;
	glm::vec3 mTarget;

	glm::mat4 mProjection;
	glm::mat4 mView;

	bool mUpdateViewNeeded;
};