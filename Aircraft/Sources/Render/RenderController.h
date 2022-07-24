#pragma once

class ShaderController;
class ShaderProgramController;
class TextureController;
class VertexArrayController;

class RenderController {
public:
	RenderController();
	ShaderController* shaderController();
	ShaderProgramController* shaderProgramController();
	TextureController* textureController();
	VertexArrayController* vertexArrayController();
protected:
	ShaderController* mShaderController;
	ShaderProgramController* mShaderProgramController;
	TextureController* mTextureController;
	VertexArrayController* mVertexArrayController;
};