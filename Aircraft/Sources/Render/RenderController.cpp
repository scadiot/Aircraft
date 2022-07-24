#include "RenderController.h"
#include "ShaderController.h"
#include "ShaderProgramController.h"
#include "TextureController.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgramDeferredLightPass.h"
#include "GBuffer.h"
#include "Quad.h"
#include "VertexArray.h"
#include "RenderObject.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

RenderController::RenderController(unsigned int windowWidth, unsigned int windowHeight)
{
	mShaderController = new ShaderController();
	mShaderProgramController = new ShaderProgramController(this);
	mTextureController = new TextureController();

	mCamera = new Camera();
    mDeferredLightingPass = new ShaderProgramDeferredLightPass(this, "deferredLightingPass.vs", "deferredLightingPass.fs");
    mGBufferShaderProgram = mShaderProgramController->loadTexture("gbuffer.vs", "gbuffer.fs");
    mGBuffer = new GBuffer(windowWidth, windowHeight);
    mQuad = new Quad();
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

Camera* RenderController::camera()
{
	return mCamera;
}

void RenderController::setLightDirection(glm::vec3 pDirection)
{
    mLightDirection = pDirection;
}

void RenderController::pushRenderObject(RenderObject* pRenderObject)
{
	mRenderObjects.push_back(pRenderObject);
}

void RenderController::render()
{
    glm::mat4 viewProjection = mCamera->getViewMatrix() * mCamera->getProjectionMatrix();

    mGBuffer->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    mGBufferShaderProgram->use();

    for (RenderObject* renderObject : mRenderObjects) {
        mGBufferShaderProgram->setMvpMatrix(renderObject->transform() * viewProjection);
        mGBufferShaderProgram->setModelMatrix(renderObject->transform());
        renderObject->vertexArray()->bind();
        renderObject->texture()->bind();
        glDrawElements(GL_TRIANGLES, renderObject->vertexArray()->facesCount() * 3, GL_UNSIGNED_INT, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    mDeferredLightingPass->use();
    auto inverseMatrix = glm::inverse(mCamera->getProjectionMatrix() * mCamera->getViewMatrix());
    mDeferredLightingPass->setInverseMatrix(inverseMatrix);
    mDeferredLightingPass->setCameraDirection(mCamera->getPosition());
    mDeferredLightingPass->setAtmsophereRadius(7);
    mDeferredLightingPass->setPlanetRadius(5);
    mDeferredLightingPass->setPlanetPosition(glm::vec3(0, 0, 0));
    mDeferredLightingPass->setSunOrientation(mLightDirection);

    mQuad->vertexArray()->bind();
    mGBuffer->bindTexture();
    glDrawElements(GL_TRIANGLES, mQuad->vertexArray()->facesCount() * 3, GL_UNSIGNED_INT, 0);
}