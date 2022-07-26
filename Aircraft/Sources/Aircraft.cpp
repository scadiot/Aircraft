#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Render/RenderController.h"
#include "Render/TextureController.h"
#include "Render/VertexArray.h"
#include "Render/Model.h"
#include "Render/Mesh.h"
#include "Render/Sphere.h"
#include "Render/Camera.h"

#include "Planet/Planet.h"

#include "Render/ShaderProgramDeferredLightPass.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static double limitFPS = 1.0 / 60.0;

float sunDirection = 0;
float cameraDirection = 0;
float cameraDistance = 20;
float cameraYaw = 20;
float cameraPitch = 20;

float keyLeftDown = false;
float keyRightDown = false;
float keyUpDown = false;
float keyDownDown = false;
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

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        keyUpDown = true;

    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        keyUpDown = false;

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        keyDownDown = true;

    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        keyDownDown = false;
}

int main(void)
{
    const int windowWidth = 640;
    const int windowHeight = 480;
    GLFWwindow* window;

    srand(time(NULL));

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




    RenderController* mRenderController = new RenderController(windowWidth, windowHeight);
    //ShaderProgram* shaderProgram = mRenderController->shaderProgramController()->loadTexture("basic.vs", "basic.ps");
  
    Space::Planet* planet = new Space::Planet(mRenderController);

    //glEnable(GL_DEPTH_TEST);
    //Model* lModel = new Model("Contents\\earth\\earth.obj");
    


    //Texture* lTexture = mRenderController->textureController()->loadTexture("earth\\Textures\\earth.png");
    //Framebuffer* framebuffer = new Framebuffer(64, 64);

    //GBuffer* lGBuffer = new GBuffer(windowWidth, windowHeight);

    //Quad* quad = new Quad();
    //Sphere* sphere = new Sphere();
    //glm::mat4 quadMatrix = glm::translate(glm::vec3(0, 0, 0));

    //Camera* camera = new Camera();

    //glm::dvec3 cameraPosition(4672169.5395881375, 4331978.5510630263, -27232.684498944320);
    glm::dvec3 cameraPosition(4675247.3258589916, 4335082.4288143180, -24222.705019162586);
    //glm::dvec3 cameraPosition(0, 0, Space::Planet::Radius * 3);
    glm::vec3 cameraTarget(0, 0, 10);
    //camera->setPosition(cameraPosition);
    //camera->setTarget(cameraTarget);

    mRenderController->camera()->setPosition(cameraPosition);
    //mRenderController->camera()->setTarget(cameraTarget);
    double lastTime = glfwGetTime(), timer = lastTime;
    double deltaTime = 0, nowTime = 0;
    //glm::rotate((double)glfwGetTime(), glm::dvec3(0, 1, 0));
    //bool show_demo_window = true;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        mRenderController->textureController()->update();

        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime);
        lastTime = nowTime;
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        //ImGui::NewFrame();

        
        //ImGui::Begin("Hello, world!");
        //ImGui::Text("This is some useful text.");
        //ImGui::End();
        if (keyRightDown) {
            sunDirection -= 0.08f * deltaTime;
        }
        if (keyLeftDown) {
            sunDirection += 0.08f * deltaTime;
        }

        if (keyADown) {
            cameraYaw += 0.008f * deltaTime;
        }
        if (keyDDown) {
            cameraYaw -= 0.008f * deltaTime;
        }

        if (keyWDown) {
            cameraPitch += 0.008f * deltaTime;
        }
        if (keySDown) {
            cameraPitch -= 0.008f * deltaTime;
        }
        
        mRenderController->camera()->setPosition(cameraPosition);
        //camera->setPosition(cameraPosition);

        //glm::mat4 m2 = glm::rotate(glm::mat4(1.0f), cameraDirection, glm::vec3(0, 1, 0));
        glm::mat4 transform = glm::eulerAngleYXZ(glm::radians(cameraYaw), glm::radians(cameraPitch), glm::radians(0.0f));
        glm::dvec3 cameraDir = glm::vec3(transform * glm::vec4(0, 0, -1, 1));
        cameraTarget = cameraDir + cameraPosition;
        mRenderController->camera()->setDirection(cameraDir);

        double distanceFromCenter = glm::length(cameraPosition);
        double speed = std::min(std::max(0.1, (distanceFromCenter - Space::Planet::Radius) / 10000.0), 1000.0);
        if (keyUpDown) {
            cameraPosition += cameraDir * speed * deltaTime;
        }
        if (keyDownDown) {
            cameraPosition -= cameraDir * speed * deltaTime;
        }

        mRenderController->camera()->setPosition(cameraPosition);// {0.0, 0.0, Space::Planet::Radius * 3});
        //ImGui::Render();
        //glm::mat4 earthModelMatrix = glm::rotate((float)glfwGetTime(), glm::vec3(0, 1, 0));
        mRenderController->setLightDirection(glm::normalize(glm::vec3(0,0.5, -1)));

        planet->update(cameraPosition);

       // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        mRenderController->render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
