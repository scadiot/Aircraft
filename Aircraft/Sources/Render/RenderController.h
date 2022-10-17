#pragma once

#include <list>
#include <glm/glm.hpp>

class ShaderController;
class ShaderProgramController;
class TextureController;
class VertexArrayController;
class RenderObject;
class Camera;
class ShaderProgramDeferredLightPass;
class ShaderProgram;
class GBuffer;
class Quad;

class RenderController {
public:
	RenderController(unsigned int windowWidth, unsigned int windowHeight);
	ShaderController* shaderController();
	ShaderProgramController* shaderProgramController();
	TextureController* textureController();
	VertexArrayController* vertexArrayController();

	Camera* camera();

	void pushRenderObject(RenderObject* pRenderObject);
	void removeRenderObject(RenderObject* pRenderObject);
	void clearRenderObject();
	uint32_t renderObjectCount();

	void render();
	void setLightDirection(glm::vec3 pDirection);
	void setPlanetRadius(float pRadius);
	void setAtmosphereRadius(float pRadius);
protected:
	ShaderController* mShaderController;
	ShaderProgramController* mShaderProgramController;
	TextureController* mTextureController;
	VertexArrayController* mVertexArrayController;

	std::list<RenderObject*> mRenderObjects;

	Camera* mCamera;
	glm::vec3 mLightDirection;

	ShaderProgramDeferredLightPass* mDeferredLightingPass;
	ShaderProgram* mGBufferShaderProgram;

	GBuffer* mGBuffer;
	Quad* mQuad;

	float mPlanetRadius;
	float mAtmosphereRadius;
};