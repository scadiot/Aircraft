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
#include "Sphere.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

RenderController::RenderController(unsigned int windowWidth, unsigned int windowHeight)
{
	mShaderController = new ShaderController();
	mShaderProgramController = new ShaderProgramController(this);
	mTextureController = new TextureController();

	mCamera = new Camera();
    mDeferredLightingPass = new ShaderProgramDeferredLightPass(this, "deferredLightingPass.vert", "deferredLightingPass.frag");
    mGBufferShaderProgram = mShaderProgramController->loadTexture("gbuffer.vert", "gbuffer.frag");
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

void RenderController::setPlanetRadius(float pRadius)
{
    mPlanetRadius = pRadius;
}

void RenderController::setAtmosphereRadius(float pRadius)
{
    mAtmosphereRadius = pRadius;
}

void RenderController::pushRenderObject(RenderObject* pRenderObject)
{
	mRenderObjects.push_back(pRenderObject);
}

void RenderController::removeRenderObject(RenderObject* pRenderObject)
{
    mRenderObjects.remove(pRenderObject);
}

void RenderController::clearRenderObject()
{
    mRenderObjects.clear();
}

uint32_t RenderController::renderObjectCount()
{
    return mRenderObjects.size();
}

void RenderController::render()
{
    glm::dmat4 view = glm::lookAt(glm::dvec3(0, 0, 0), mCamera->getDirection(), glm::dvec3(0, 1, 0));
    glm::dmat4 viewProjection = mCamera->getProjectionMatrix() * view;
    glm::dmat4 decal = glm::translate(glm::dmat4(1), mCamera->getPosition() * -1.0);


    mGBuffer->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mGBufferShaderProgram->use();
    for (RenderObject* renderObject : mRenderObjects) {
        mGBufferShaderProgram->setMvpMatrix((glm::mat4)(viewProjection * decal * renderObject->transform()));
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
    auto inverseMatrix = glm::inverse(viewProjection);
    mDeferredLightingPass->setInverseMatrix(inverseMatrix);
    mDeferredLightingPass->setCameraDirection(mCamera->getPosition());
    mDeferredLightingPass->setAtmsophereRadius(mAtmosphereRadius);
    mDeferredLightingPass->setPlanetRadius(mPlanetRadius);
    mDeferredLightingPass->setPlanetPosition(glm::vec3(0, 0, 0));
    mDeferredLightingPass->setSunOrientation(mLightDirection);

    mQuad->vertexArray()->bind();
    mGBuffer->bindTexture();
    glDrawElements(GL_TRIANGLES, mQuad->vertexArray()->facesCount() * 3, GL_UNSIGNED_INT, 0);
}