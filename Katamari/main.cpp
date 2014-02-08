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
    
    // Load vertex and fragment shaders
    GLuint vertexShader = loadShaderFromFile("VertexTest.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShaderFromFile("FragmentTest.glsl", GL_FRAGMENT_SHADER);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Prepare model matrix
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");

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
    glDepthFunc(GL_LESS);     // Accept fragment if it closer to the camera than the former one

    // Test objects
    Triangle triangle(shaderProgram);
    Cube cube(shaderProgram);
    Line line(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Position & draw the triangle
        Matrix4x4 t = Translation(Vector3(0,0,-10));
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, t.Transpose().Start());
        triangle.Render();

        // Position & draw the cube
        t = Translation(Vector3(-2,2,-10));
        Matrix4x4 r = Rotation(45, AXIS_Y);
        r = r*Rotation(45, AXIS_X);
        Matrix4x4 m = t*r;
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, m.Transpose().Start());
        cube.Render();

        // Position & draw the cube again
        t = Translation(Vector3(4,0,-10));
        r = Rotation(45, AXIS_Y);
        r = r*Rotation(45, AXIS_X);
        m = t*r;
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, m.Transpose().Start());
        cube.Render();

        // Position & draw the line
        t = Translation(Vector3(0,0,-5));
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, t.Transpose().Start());
        line.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    triangle.Cleanup();
    cube.Cleanup();

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

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