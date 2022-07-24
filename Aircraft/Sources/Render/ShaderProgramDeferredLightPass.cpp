#include "ShaderProgramDeferredLightPass.h"

ShaderProgramDeferredLightPass::ShaderProgramDeferredLightPass(RenderController* pRenderController, std::string pVertexPath, std::string pFragmentPath) :
	ShaderProgram(pRenderController, pVertexPath, pFragmentPath)
{
	mInverseMatrixLocation = glGetUniformLocation(mShaderProgramId, "inverseMatrix");
	mCameraPositionLocation = glGetUniformLocation(mShaderProgramId, "gCameraPosition");
	mPlanetPositionLocation = glGetUniformLocation(mShaderProgramId, "gPlanetPosition");
	mPlanetRadiusLocation = glGetUniformLocation(mShaderProgramId, "gPlanetRadius");
	mAtmsophereRadiusLocation = glGetUniformLocation(mShaderProgramId, "gAtmsophereRadius");
	mSunOrientationLocation = glGetUniformLocation(mShaderProgramId, "gSunOrientation");

	mTexturePositionLocation = glGetUniformLocation(mShaderProgramId, "gPosition");
	mTextureNormalLocation = glGetUniformLocation(mShaderProgramId, "gNormal");
	mTextureAlbedoSpecLocation = glGetUniformLocation(mShaderProgramId, "gAlbedoSpec");
}

void ShaderProgramDeferredLightPass::setInverseMatrix(const glm::mat4& pMatrix)
{
	glUniformMatrix4fv(mInverseMatrixLocation, 1, GL_FALSE, &pMatrix[0][0]);
}

void ShaderProgramDeferredLightPass::setCameraDirection(const glm::vec3& pPosition)
{
	glUniform3fv(mCameraPositionLocation, 1, &pPosition[0]);
}

void ShaderProgramDeferredLightPass::setPlanetPosition(const glm::vec3& pPosition)
{
	glUniform3fv(mPlanetPositionLocation, 1, &pPosition[0]);
}

void ShaderProgramDeferredLightPass::setPlanetRadius(float pRadius)
{
	glUniform1f(mPlanetRadiusLocation, pRadius);
}

void ShaderProgramDeferredLightPass::setAtmsophereRadius(float pRadius)
{
	glUniform1f(mAtmsophereRadiusLocation, pRadius);
}

void ShaderProgramDeferredLightPass::setSunOrientation(const glm::vec3& pOrientation)
{
	glUniform3fv(mSunOrientationLocation, 1, &pOrientation[0]);
}

void ShaderProgramDeferredLightPass::use()
{
	ShaderProgram::use();

	glUniform1i(mTexturePositionLocation, 0);
	glUniform1i(mTextureNormalLocation, 1);
	glUniform1i(mTextureAlbedoSpecLocation, 2);
}