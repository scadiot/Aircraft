#pragma once

#include "ShaderProgram.h"

class ShaderProgramDeferredLightPass : public ShaderProgram {
public:
	ShaderProgramDeferredLightPass(RenderController* pRenderController, std::string pVertexPath, std::string pFragmentPath);
	void use() override;
	void setInverseMatrix(const glm::mat4& pMatrix);
	void setCameraDirection(const glm::vec3& pPosition);

	void setPlanetPosition(const glm::vec3& pPosition);
	void setPlanetRadius(float pRadius);
	void setAtmsophereRadius(float pRadius);
	void setSunOrientation(const glm::vec3& pOrientation);
	
protected:
	GLuint mInverseMatrixLocation;
	GLuint mCameraPositionLocation;
	GLuint mPlanetPositionLocation;
	GLuint mPlanetRadiusLocation;
	GLuint mAtmsophereRadiusLocation;

	GLuint mTexturePositionLocation;
	GLuint mTextureNormalLocation;
	GLuint mTextureAlbedoSpecLocation;
	GLuint mSunOrientationLocation;

};