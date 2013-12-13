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
    
    // cube with random vertex colours
    static const GLfloat g_vertex_buffer_data[] = 
    {
        -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, // 0
        -1.0f,-1.0f, 1.0f, 1.0f, 0.5f, 0.0f, // 1
        -1.0f, 1.0f,-1.0f, 1.0f, 0.1f, 0.0f, // 2
        -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f, // 3
         1.0f,-1.0f,-1.0f, 1.0f, 0.1f, 0.1f, // 4
         1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.1f, // 5
         1.0f, 1.0f,-1.0f, 1.0f, 0.5f, 0.5f, // 6
         1.0f, 1.0f, 1.0f, 0.0f, 0.1f, 0.1f  // 7
    };

    GLuint elements[] =
    {
        0, 1, 3,
        6, 0, 2,
        5, 0, 4,
        6, 4, 0,
        0, 3, 2,
        5, 1, 0,
        3, 1, 5,
        7, 4, 6,
        4, 7, 5,
        7, 6, 2,
        7, 2, 3,
        7, 3, 5
    };

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);                  // Generate buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);     // Make buffer active
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); // Copy data to buffer

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    GLuint vertexShader = loadShaderFromFile("VertexTest.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShaderFromFile("FragmentTest.glsl", GL_FRAGMENT_SHADER);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint uniModel = glGetUniformLocation(shaderProgram, "model");

    // prepare view matrix
    Matrix4x4 view = LookAt(Vector3(0.0, 0.0, 0.5),
                            Vector3(0.0, 0.0, 0.0),
                            Vector3(0.0, 1.0, 0.0));
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, view.Transpose().Start());

    // prepare projection matrix
    Matrix4x4 proj = PerspectiveProjection(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj.Transpose().Start());

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                            6*sizeof(float), (void*)(3*sizeof(float)));

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);     // Accept fragment if it closer to the camera than the former one

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw a test cube
        Matrix4x4 t = Translation(Vector3(0,0,-10));
        Matrix4x4 r = Rotation(45, AXIS_Y);
        r = r*Rotation(45, AXIS_X);
        Matrix4x4 m = t*r;
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, m.Transpose().Start());
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // draw another test cube
        t = Translation(Vector3(4,0,-10));
        r = Rotation(45, AXIS_Y);
        r = r*Rotation(45, AXIS_X);
        m = t*r;
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, m.Transpose().Start());
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDisableVertexAttribArray(posAttrib);
    glDisableVertexAttribArray(colAttrib);

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

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
