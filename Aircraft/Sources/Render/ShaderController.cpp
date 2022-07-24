#include "Shader.h"
#include "ShaderController.h"

ShaderController::ShaderController()
{

}

Shader* ShaderController::loadShader(std::string pName, ShaderType pType)
{
	std::string path = "shaders\\" + pName;
	Shader* shader = new Shader(path, pType);
	mShaders.push_back(shader);

	return shader;
}