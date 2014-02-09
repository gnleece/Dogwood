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
#include "Engine\Primitive.h"
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
    GLuint vertexShader = LoadShaderFromFile("Engine\\Shaders\\VertexShader.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = LoadShaderFromFile("Engine\\Shaders\\FragmentShader.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgram = LinkShaderProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    // Prepare view matrix
    Matrix4x4 view = LookAt(Vector3(0.0, 0.0, 0.5),
                            Vector3(0.0, 0.0, 0.0),
                            Vector3(0.0, 1.0, 0.0));
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, view.Transpose().Start());

    // Prepare projection matrix
    Matrix4x4 proj = PerspectiveProjection(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj.Transpose().Start());

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Test textures
    Texture tex("Engine\\Assets\\test_texture.bmp");
    Texture tex2("Engine\\Assets\\test_texture2.bmp");

    // Test objects
    Matrix4x4 trans;

    Triangle triangle(shaderProgram);
    trans = Translation(Vector3(0,0,-10));
    triangle.SetTransform(trans);

    Cube cube(shaderProgram);
    trans = Translation(Vector3(-1,1,-5));
    Matrix4x4 r = Rotation(45, AXIS_Y);
    r = r*Rotation(45, AXIS_X);
    trans = trans*r;
    cube.SetTransform(trans);
    cube.SetTexture(&tex);

    Cube cube2(shaderProgram);     // TODO share underlying primitive data between copies
    Matrix4x4 trans2 = Translation(Vector3(2,2,-10));
    Matrix4x4 r2 = Rotation(45, AXIS_Y);
    r2 = r2*Rotation(45, AXIS_X);
    trans2 = trans2*r2;
    cube2.SetTransform(trans2);
    cube2.SetTexture(&tex2);

    Line line(shaderProgram);
    trans = Translation(Vector3(0,0,-5));
    line.SetTransform(trans);

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw test objects
        cube.Render();
        cube2.Render();
        triangle.Render();
        line.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    triangle.Cleanup();     // TODO objects need to auto-cleaup
    cube.Cleanup();
    cube2.Cleanup();
    line.Cleanup();

    tex.FreeTexture();
    tex2.FreeTexture();

    glDeleteProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Cleanup(window);
    exit(EXIT_SUCCESS);
}

GLFWwindow* Setup()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Katamari", NULL, NULL);
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