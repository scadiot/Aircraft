#include "RenderController.h"
#include "ShaderController.h"
#include "ShaderProgramController.h"
#include "TextureController.h"
#include "Mesh.h"

RenderController::RenderController()
{
	mShaderController = new ShaderController();
	mShaderProgramController = new ShaderProgramController(this);
	mTextureController = new TextureController();
}

ShaderController* RenderController::shaderController()
{
	return mShaderController;
}

ShaderProgramController* RenderController::shaderProgramController()
{
	return mShaderProgramController;
}

TextureController* RenderController::textureController()
{
	return mTextureController;
}


VertexArrayController* RenderController::vertexArrayController()
{
	return mVertexArrayController;
}