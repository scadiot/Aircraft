#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Render/RenderController.h"
#include "Render/VertexArray.h"
#include "Render/Model.h"
#include "Render/Mesh.h"
#include "Render/ShaderProgramController.h"
#include "Render/TextureController.h"
#include "Render/Framebuffer.h"
#include "Render/GBuffer.h"
#include "Render/Quad.h"
#include "Render/Sphere.h"
#include "Render/Camera.h"

#include "Render/ShaderProgramDeferredLightPass.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

float sunDirection = 0;
float cameraDirection = 0;
float cameraDistance = 20;

float keyLeftDown = false;
float keyRightDown = false;

float keyWDown = false;
float keySDown = false;
float keyADown = false;
float keyDDown = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        keyLeftDown = true;

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        keyLeftDown = false;

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        keyRightDown = true;

    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        keyRightDown = false;

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        keyWDown = true;

    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        keyWDown = false;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        keySDown = true;

    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        keySDown = false;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        keyADown = true;

    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        keyADown = false;

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        keyDDown = true;

    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        keyDDown = false;
}

int main(void)
{
    const int windowWidth = 1024;
    const int windowHeight = 768;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    const char* glsl_version = "#version 130";

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);




    RenderController* mRenderController = new RenderController();
    ShaderProgram* shaderProgram = mRenderController->shaderProgramController()->loadTexture("basic.vs", "basic.ps");
  
    ShaderProgramDeferredLightPass* deferredLightingPass = new ShaderProgramDeferredLightPass(mRenderController, "deferredLightingPass.vs", "deferredLightingPass.fs");// mRenderController->shaderProgramController()->loadTexture("deferredLightingPass.vs", "deferredLightingPass.fs");
    ShaderProgram* gBufferShaderProgram = mRenderController->shaderProgramController()->loadTexture("gbuffer.vs", "gbuffer.fs");

    //glEnable(GL_DEPTH_TEST);
    Model* lModel = new Model("Contents\\earth\\earth.obj");
    


    Texture* lTexture = mRenderController->textureController()->loadTexture("earth\\Textures\\earth.png");
    Framebuffer* framebuffer = new Framebuffer(64, 64);

    GBuffer* lGBuffer = new GBuffer(windowWidth, windowHeight);

    Quad* quad = new Quad();
    Sphere* sphere = new Sphere();
    glm::mat4 quadMatrix = glm::translate(glm::vec3(0, 0, 0));

    Camera* camera = new Camera();
    glm::vec3 cameraPosition(0, 0, 30);
    glm::vec3 cameraTarget(0, 0, 0);
    camera->setPosition(cameraPosition);
    camera->setTarget(cameraTarget);

    glm::rotate((double)glfwGetTime(), glm::dvec3(0, 1, 0));
    bool show_demo_window = true;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        
        //ImGui::Begin("Hello, world!");
        //ImGui::Text("This is some useful text.");
        //ImGui::End();
        if (keyRightDown) {
            sunDirection -= 0.05f;
        }
        if (keyLeftDown) {
            sunDirection += 0.05f;
        }

        if (keyADown) {
            cameraDirection += 0.01f;
        }
        if (keyDDown) {
            cameraDirection -= 0.01f;
        }

        camera->setPosition(cameraPosition);

        glm::mat4 m2 = glm::rotate(glm::mat4(1.0f), cameraDirection, glm::vec3(0, 1, 0));
        glm::vec3 cameraDir = glm::vec3(m2 * glm::vec4(0, 0, -1, 1));
        cameraTarget = cameraDir + cameraPosition;
        camera->setTarget(cameraTarget);



        if (keyWDown) {
            cameraPosition += cameraDir * 0.01f;
        }
        if (keySDown) {
            cameraPosition -= cameraDir * 0.01f;
        }
        camera->setPosition(cameraPosition);
        ImGui::Render();
        glm::mat4 earthModelMatrix = glm::rotate((float)glfwGetTime(), glm::vec3(0, 1, 0));
        glm::mat4 earthTransformMatrix = camera->getProjectionMatrix() * camera->getViewMatrix() * earthModelMatrix;

        
        lGBuffer->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        gBufferShaderProgram->use();
        gBufferShaderProgram->setMvpMatrix(earthTransformMatrix);
        gBufferShaderProgram->setModelMatrix(earthModelMatrix);
        lTexture->bind();

        //for (Mesh* mesh : lModel->meshes()) {
        //    mesh->vertexArray()->bind();
        //    glDrawElements(GL_TRIANGLES, mesh->vertexArray()->facesCount() * 3, GL_UNSIGNED_INT, 0);
        //}
        sphere->vertexArray()->bind();
        glDrawElements(GL_TRIANGLES, sphere->vertexArray()->facesCount() * 3, GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        deferredLightingPass->use();
        auto inverseMatrix = glm::inverse(camera->getProjectionMatrix() * camera->getViewMatrix());
        deferredLightingPass->setInverseMatrix(inverseMatrix);
        deferredLightingPass->setCameraDirection(cameraPosition);
        deferredLightingPass->setAtmsophereRadius(7);
        deferredLightingPass->setPlanetRadius(5);
        deferredLightingPass->setPlanetPosition(glm::vec3(0, 0, 0));

        glm::mat4 m = glm::rotate(glm::mat4(1.0f), sunDirection, glm::vec3(0, 1, 0));
        glm::vec3 sunDirectionV = glm::normalize(glm::vec3(m * glm::vec4(0, 0, -1, 1)) + glm::vec3(0, 0.7, 0));
        deferredLightingPass->setSunOrientation(sunDirectionV);
        //deferredLightingPass->setMvpMatrix(quadMatrix);
        quad->vertexArray()->bind();
        lGBuffer->bindTexture();
        glDrawElements(GL_TRIANGLES, quad->vertexArray()->facesCount() * 3, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
