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
    GLuint vertexColourShader = LoadShaderFromFile("Engine\\Shaders\\VertexColour.glsl", GL_VERTEX_SHADER);
    GLuint fragmentColourShader = LoadShaderFromFile("Engine\\Shaders\\FragmentColour.glsl", GL_FRAGMENT_SHADER);

    GLuint vertexTextureShader = LoadShaderFromFile("Engine\\Shaders\\VertexTexture.glsl", GL_VERTEX_SHADER);
    GLuint fragmentTextureShader = LoadShaderFromFile("Engine\\Shaders\\FragmentTexture.glsl", GL_FRAGMENT_SHADER);

    // Link the common vertex and fragment shaders into shader programs
    GLuint colourShaderProgram = LinkShaderProgram(vertexColourShader, fragmentColourShader);
    GLuint textureShaderProgram = LinkShaderProgram(vertexTextureShader, fragmentTextureShader);

    glUseProgram(colourShaderProgram); 

    // Prepare view matrix
    Matrix4x4 view = LookAt(Vector3(0.0, 0.0, 0.5),
                            Vector3(0.0, 0.0, 0.0),
                            Vector3(0.0, 1.0, 0.0));
    GLint uniView = glGetUniformLocation(colourShaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, view.Transpose().Start());
    uniView = glGetUniformLocation(textureShaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, view.Transpose().Start());

    // Prepare projection matrix
    Matrix4x4 proj = PerspectiveProjection(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    GLint uniProj = glGetUniformLocation(colourShaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj.Transpose().Start());
    uniProj = glGetUniformLocation(textureShaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj.Transpose().Start());

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);     // Accept fragment if it closer to the camera than the former one

    // Test objects
    Matrix4x4 trans;

    Triangle triangle(colourShaderProgram);
    trans = Translation(Vector3(0,0,-10));
    triangle.SetTransform(trans);

    Cube cube(colourShaderProgram);
    trans = Translation(Vector3(-2,2,-10));
    Matrix4x4 r = Rotation(45, AXIS_Y);
    r = r*Rotation(45, AXIS_X);
    trans = trans*r;
    cube.SetTransform(trans);

    Cube cube2(colourShaderProgram);     // TODO share underlying primitive data between copies
    trans = Translation(Vector3(4,0,-10));
    r = Rotation(45, AXIS_Y);
    r = r*Rotation(45, AXIS_X);
    trans = trans*r;
    cube2.SetTransform(trans);

    Line line(colourShaderProgram);
    trans = Translation(Vector3(0,0,-5));
    line.SetTransform(trans);

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw test objects
        triangle.Render();
        cube.Render();
        cube2.Render();
        line.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    triangle.Cleanup();     // TODO objects need to auto-cleaup
    cube.Cleanup();
    cube2.Cleanup();
    line.Cleanup();

    glDeleteProgram(colourShaderProgram);
    glDeleteProgram(textureShaderProgram);

    glDeleteShader(vertexColourShader);
    glDeleteShader(fragmentColourShader);
    glDeleteShader(vertexTextureShader);
    glDeleteShader(fragmentTextureShader);

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