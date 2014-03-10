// main.cpp : Defines the entry point for the console application.
//

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Engine\Light.h"
#include "Engine\Math\Algebra.h"
#include "Engine\Math\Transformations.h"
#include "Engine\Primitive.h"
#include "Engine\ShaderProgram.h"
#include "Engine\Texture.h"
#include "Engine\Util.h"

GLFWwindow* Setup();
void Cleanup(GLFWwindow* window);

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    GLFWwindow* window = Setup();
    
    // Load common vertex and fragment shaders
	ShaderProgram shaderProgram("Engine\\Shaders\\VertexShader.glsl", "Engine\\Shaders\\FragmentShader.glsl");
	glUseProgram(shaderProgram.GetID());

    // Prepare view matrix
    Matrix4x4 view = LookAt(Vector3(0.0, 0.0, 0.5),
                            Vector3(0.0, 0.0, 0.0),
                            Vector3(0.0, 1.0, 0.0));
    GLint uniView = glGetUniformLocation(shaderProgram.GetID(), "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, view.Transpose().Start());

    // Prepare projection matrix
    Matrix4x4 proj = PerspectiveProjection(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram.GetID(), "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj.Transpose().Start());

    // LIGHT SETUP
    Vector3 lightPosition(0.0f, 0.0f, 0.0f);
    Vector3 lightColor(1.0f, 1.0f, 1.0f);
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

    Cube cube(shaderProgram);
    trans = Translation(Vector3(-2,-0.2,-5));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    scale = UniformScaling(1.2f);
    trans = trans*rot*scale;
    cube.SetTransform(trans);
    cube.SetColour(ColourRGB::White);

    Cube cube2(shaderProgram);
    trans = Translation(Vector3(0.5,-0.5,-5));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    trans = trans*rot;
    cube2.SetTransform(trans);
    cube2.SetTexture(&tex2);
    cube2.SetColour(ColourRGB::Green);

    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw test objects
        triangle.Render();
        triangle2.Render();
        triangle3.Render();
        cube.Render();
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
    cube.Cleanup();
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