#pragma once

#include "ShaderProgram.h"
#include <vector>

class RenderController;

class ShaderProgramController {
public:
	ShaderProgramController(RenderController* pRenderController);
	ShaderProgram* loadTexture(std::string pVertexPath, std::string pFragmentPath);
protected:
	RenderController* mRenderController;
	std::vector<ShaderProgram*> mShaderPrograms;
};