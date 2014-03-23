// main.cpp : Defines the entry point for the console application.
//

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Engine\Math\Algebra.h"
#include "Engine\Math\Transformations.h"
#include "Engine\Mesh.h"
#include "Engine\Primitive.h"
#include "Engine\Rendering\Light.h"
#include "Engine\Rendering\ShaderProgram.h"
#include "Engine\Texture.h"
#include "Engine\Util.h"

GLFWwindow* Setup();
void Cleanup(GLFWwindow* window);

// TODO move this somewhere better
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_callback(GLFWwindow* window, double x, double y);

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

int main(void)
{
    GLFWwindow* window = Setup();

    // Load common vertex and fragment shaders
	ShaderProgram shaderProgram("Engine\\Shaders\\VertexShader.glsl", "Engine\\Shaders\\FragmentShader.glsl");
	glUseProgram(shaderProgram.GetID());

    // Prepare view matrix
    MainCamera.position = Vector3(0.0, 0.0, 0.0);
    MainCamera.direction = Vector3(0.0, 0.0, -1.0);
    MainCamera.up = Vector3(0.0, 1.0, 0.0);
    viewMatrix = LookAt(MainCamera);
    uniView = glGetUniformLocation(shaderProgram.GetID(), "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, viewMatrix.Transpose().Start());

    // Prepare projection matrix
    Matrix4x4 proj = PerspectiveProjection(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram.GetID(), "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj.Transpose().Start());

    // LIGHT SETUP
    Vector3 lightPosition(0.0f, 0.0f, 0.0f);
    ColourRGB lightColor(1.0f, 1.0f, 1.0f);
    GLfloat lightPower = 15.0f;
    Light light(lightPosition, lightColor, lightPower);
    light.SetLightForShader(shaderProgram.GetID());

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Test textures
    Texture tex("Engine\\Assets\\test_texture.bmp");
    Texture tex2("Engine\\Assets\\test_texture2.bmp");

    // Test objects
    Matrix4x4 trans;
    Matrix4x4 rot;
    Matrix4x4 scale;

    double startTime = glfwGetTime();
    printf("Start time %f\n", startTime);
    Triangle triangle(shaderProgram);
    trans = Translation(Vector3(0,1,-5));
    scale = UniformScaling(0.5f);
    triangle.SetTransform(trans*scale);
    triangle.SetColour(ColourRGB::Cyan);

    Triangle triangle2(shaderProgram);
    trans = Translation(Vector3(1,1,-5));
    scale = UniformScaling(0.5f);
    triangle2.SetTransform(trans*scale);
    triangle2.SetColour(ColourRGB::Yellow);

    Triangle triangle3(shaderProgram);
    trans = Translation(Vector3(2,1,-5));
    scale = UniformScaling(0.5f);
    triangle3.SetTransform(trans*scale);
    triangle3.SetColour(ColourRGB::Magenta);

    /*
    Cube cube(shaderProgram);
    trans = Translation(Vector3(-1.2,-0.25,-5));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    scale = UniformScaling(0.6f);
    trans = trans*rot*scale;
    cube.SetTransform(trans);
    cube.SetColour(ColourRGB::White);
    */

    Sphere sphere(shaderProgram);
    trans = Translation(Vector3(-1.0f, -0.5f, -5.0f));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    scale = UniformScaling(0.8f);
    trans = trans*rot*scale;
    sphere.SetTransform(trans);
    sphere.SetColour(ColourRGB::White);
    sphere.SetColour(ColourRGB::Yellow);

    Cube cube2(shaderProgram);
    trans = Translation(Vector3(1.2f,-0.5f,-5.0f));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    scale = UniformScaling(0.45f);
    trans = trans*rot*scale;
    cube2.SetTransform(trans);
    cube2.SetTexture(&tex2);
    cube2.SetColour(ColourRGB::Green);

    double prevTime = glfwGetTime();
    printf("Done time %f\n", prevTime);
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw test objects
        triangle.Render();
        triangle2.Render();
        triangle3.Render();
        //cube.Render();
        sphere.Render();
        cube2.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        double currentTime = glfwGetTime();
        double frameTime = currentTime - prevTime;
        double FPS = 1 / frameTime;
        //printf("Frame time: %f\t\tFPS: %3.f\n", frameTime, FPS);
        prevTime = currentTime;
    }

    triangle.Cleanup();     // TODO objects need to auto-cleaup
    triangle2.Cleanup();
    triangle3.Cleanup();
    //cube.Cleanup();
    sphere.Cleanup();
    cube2.Cleanup();

    tex.FreeTexture();
    tex2.FreeTexture();

    shaderProgram.Delete();

    Cleanup(window);
    exit(EXIT_SUCCESS);
}

GLFWwindow* Setup()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Katamari", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_callback);

    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glewExperimental = GL_TRUE;
    glewInit();

    return window;
}

void Cleanup(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
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
    float rotAmt;
    if (mouseDragging)
    {
        double deltaX = x - prevX;
        double deltaY = y - prevY;
        RotateCamera(AXIS_Y, deltaX*0.07);
        RotateCamera(AXIS_X, deltaY*0.07);
    }
    prevX = x;
    prevY = y;
}