#pragma once

#include "Shader.h"
#include <vector>

class ShaderController {
public:
	ShaderController();
	Shader* loadShader(std::string pName, ShaderType pType);
protected:
	std::vector<Shader*> mShaders;
};