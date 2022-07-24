#include "ShaderProgramController.h"

ShaderProgramController::ShaderProgramController(RenderController* pRenderController)
{
	mRenderController = pRenderController;
}

ShaderProgram* ShaderProgramController::loadTexture(std::string pVertexPath, std::string pFragmentPath)
{
	ShaderProgram* shaderPrograms = new ShaderProgram(mRenderController, pVertexPath, pFragmentPath);
	mShaderPrograms.push_back(shaderPrograms);

	return shaderPrograms;
}
