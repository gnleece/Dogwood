#include "Game.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Math\Algebra.h"
#include "Math\Transformations.h"
#include "Rendering\Light.h"
#include "Rendering\Mesh.h"
#include "Rendering\ShaderProgram.h"
#include "Rendering\Texture.h"
#include "Util.h"

// TODO ugh temp hack
Camera MainCamera;
GLint uniView;
Matrix4x4 viewMatrix;

void MoveCamera(Vector3 localSpaceOffset)
{
    viewMatrix = Translation(localSpaceOffset)*viewMatrix;
    glUniformMatrix4fv(uniView, 1, GL_FALSE, viewMatrix.Transpose().Start());
}

void RotateCamera(eAXIS axis, float degrees)
{
    viewMatrix = Rotation(degrees, axis)*viewMatrix;
    glUniformMatrix4fv(uniView, 1, GL_FALSE, viewMatrix.Transpose().Start());
}

// TODO move this somewhere better
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_callback(GLFWwindow* window, double x, double y);

Game::Game(std::string name, int windowWidth, int windowHeight) : 
    m_name(name), m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
    WindowSetup();
    RenderingSetup();
}

void Game::Run()
{
    // LIGHT SETUP
    Vector3 lightPosition(0.0f, 0.0f, 0.0f);
    ColourRGB lightColor(1.0f, 1.0f, 1.0f);
    GLfloat lightPower = 15.0f;
    Light light(lightPosition, lightColor, lightPower);
    light.SetLightForShader(m_shaderProgram.GetID());

    // Test textures
    Texture tex("Engine\\Assets\\test_texture.bmp");
    Texture tex2("Engine\\Assets\\test_texture2.bmp");

    // Test objects
    Matrix4x4 trans;
    Matrix4x4 rot;
    Matrix4x4 scale;

    double startTime = glfwGetTime();
    printf("Start time %f\n", startTime);

    Mesh sphere("Engine\\Assets\\Models\\sphere.obj", m_shaderProgram);
    trans = Translation(Vector3(-1.0f, -0.5f, -5.0f));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    scale = UniformScaling(0.8f);
    trans = trans*rot*scale;
    sphere.SetTransform(trans);
    sphere.SetColour(ColourRGB::White);
    sphere.SetColour(ColourRGB::Yellow);

    Mesh cube("Engine\\Assets\\Models\\cube.obj", m_shaderProgram);
    trans = Translation(Vector3(1.2f, -0.5f, -5.0f));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    scale = UniformScaling(0.45f);
    trans = trans*rot*scale;
    cube.SetTransform(trans);
    cube.SetTexture(&tex2);
    cube.SetColour(ColourRGB::Green);

    double prevTime = glfwGetTime();
    printf("Done time %f\n", prevTime);

    while (!glfwWindowShouldClose(m_window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw test objects
        sphere.Render();
        cube.Render();

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        double currentTime = glfwGetTime();
        double frameTime = currentTime - prevTime;
        double FPS = 1 / frameTime;
        //printf("Frame time: %f\t\tFPS: %3.f\n", frameTime, FPS);
        prevTime = currentTime;
    }

    sphere.Delete();    // TODO objects need to auto cleanup
    cube.Delete();

    tex.FreeTexture();
    tex2.FreeTexture();
}

void Game::Shutdown()
{
    m_shaderProgram.Delete();
    WindowCleanup();
    exit(EXIT_SUCCESS);
}

void Game::WindowSetup()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_name.c_str(), NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouse_callback);
    glfwSetCursorPosCallback(m_window, cursor_callback);

    glewExperimental = GL_TRUE;
    glewInit();
}

void Game::RenderingSetup()
{
    // Load common vertex and fragment shaders
    m_shaderProgram.Load("Engine\\Shaders\\VertexShader.glsl", "Engine\\Shaders\\FragmentShader.glsl");
    glUseProgram(m_shaderProgram.GetID());

    // Prepare view matrix
    MainCamera.position = Vector3(0.0, 0.0, 0.0);
    MainCamera.direction = Vector3(0.0, 0.0, -1.0);
    MainCamera.up = Vector3(0.0, 1.0, 0.0);
    viewMatrix = LookAt(MainCamera);
    uniView = glGetUniformLocation(m_shaderProgram.GetID(), "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, viewMatrix.Transpose().Start());

    // Prepare projection matrix
    Matrix4x4 proj = PerspectiveProjection(45.0f, (float)m_windowWidth/m_windowHeight, 0.1f, 1000.0f);
    GLint uniProj = glGetUniformLocation(m_shaderProgram.GetID(), "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj.Transpose().Start());

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Game::WindowCleanup()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float rotAmt = 1.0f;
    float transAmt = 0.2f;
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_UP:
        RotateCamera(AXIS_X, -rotAmt);
        break;
    case GLFW_KEY_DOWN:
        RotateCamera(AXIS_X, rotAmt);
        break;
    case GLFW_KEY_LEFT:
        RotateCamera(AXIS_Y, -rotAmt);
        break;
    case GLFW_KEY_RIGHT:
        RotateCamera(AXIS_Y, rotAmt);
        break;
    case GLFW_KEY_W:
        MoveCamera(Vector3(0, 0, transAmt));
        break;
    case GLFW_KEY_S:
        MoveCamera(Vector3(0, 0, -transAmt));
        break;
    case GLFW_KEY_A:
        MoveCamera(Vector3(transAmt, 0, 0));
        break;
    case GLFW_KEY_D:
        MoveCamera(Vector3(-transAmt, 0, 0));
        break;
    };
}

bool mouseDragging = false;
double prevX;
double prevY;

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    printf("Button: %d   Actions: %d\n", button, action);
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            mouseDragging = true;
            glfwGetCursorPos(window, &prevX, &prevY);
        }
        else
        {
            mouseDragging = false;
        }
    }
}

static void cursor_callback(GLFWwindow* window, double x, double y)
{
    float rotAmt = 0.07;
    if (mouseDragging)
    {
        double deltaX = x - prevX;
        double deltaY = y - prevY;
        RotateCamera(AXIS_Y, deltaX*rotAmt);
        RotateCamera(AXIS_X, deltaY*rotAmt);
    }
    prevX = x;
    prevY = y;
}