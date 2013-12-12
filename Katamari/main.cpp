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
    // TODO this is super redundant. reuse vertices!
    static const GLfloat g_vertex_buffer_data[] = 
    {
        -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f,// triangle 1 : begin
        -1.0f,-1.0f, 1.0f, 1.0f, 0.2f, 0.0f,
        -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.2f,// triangle 1 : end
        1.0f, 1.0f,-1.0f, 1.0f, 0.2f, 0.2f,// triangle 2 : begin
        -1.0f,-1.0f,-1.0f,1.0f, 0.4f, 0.0f,
        -1.0f, 1.0f,-1.0f,1.0f, 0.0f, 0.4f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,1.0f, 0.4f, 0.4f,
        -1.0f,-1.0f,-1.0f,1.0f, 0.6f, 0.0f,
        1.0f,-1.0f,-1.0f,1.0f, 0.0f, 0.6f,
        1.0f, 1.0f,-1.0f,1.0f, 0.6f, 0.6f,
        1.0f,-1.0f,-1.0f,1.0f, 0.8f, 0.0f,
        -1.0f,-1.0f,-1.0f,1.0f, 0.0f, 0.8f,
        -1.0f,-1.0f,-1.0f,1.0f, 0.8f, 0.8f,
        -1.0f, 1.0f, 1.0f,1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,1.0f, 0.0f, 1.0f,
        1.0f,-1.0f, 1.0f,1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,0.0f, 1.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,0.0f, 0.2f, 0.0f,
        -1.0f,-1.0f, 1.0f,0.0f, 0.0f, 0.2f,
        1.0f,-1.0f, 1.0f,0.0f, 0.2f, 0.2f,
        1.0f, 1.0f, 1.0f,0.0f, 0.4f, 0.0f,
        1.0f,-1.0f,-1.0f,0.0f, 0.0f, 0.4f,
        1.0f, 1.0f,-1.0f,0.0f, 0.4f, 0.4f,
        1.0f,-1.0f,-1.0f,0.0f, 0.6f, 0.0f,
        1.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.6f,
        1.0f,-1.0f, 1.0f,0.0f, 0.6f, 0.6f,
        1.0f, 1.0f, 1.0f,0.0f, 0.8f, 0.0f,
        1.0f, 1.0f,-1.0f,0.0f, 0.0f, 0.8f,
        -1.0f, 1.0f,-1.0f,0.0f, 0.8f, 0.8f,
        1.0f, 1.0f, 1.0f,0.5f, 0.2f, 0.0f,
        -1.0f, 1.0f,-1.0f,0.5f, 0.0f, 0.2f,
        -1.0f, 1.0f, 1.0f,0.5f, 0.2f, 0.2f,
        1.0f, 1.0f, 1.0f,0.5f, 0.4f, 0.0f,
        -1.0f, 1.0f, 1.0f,0.5f, 0.0f, 0.4f,
        1.0f,-1.0f, 1.0f,0.5f, 0.4f, 0.4f,
    };

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);                  // Generate buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);     // Make buffer active
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); // Copy data to buffer

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
    Matrix4x4 view = Matrix4x4::LookAt(Vector3(0.0, 0.0, 0.5),
                                       Vector3(0.0, 0.0, 0.0),
                                       Vector3(0.0, 1.0, 0.0));
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, view.Transpose().Start());

    // prepare (perspective) projection matrix
    Matrix4x4 proj = Matrix4x4::Projection(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj.Transpose().Start());

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);     // Accept fragment if it closer to the camera than the former one

    while (!glfwWindowShouldClose(window))
    {
        // Specify the layout of the vertex data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);

        GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                               6*sizeof(float), (void*)(3*sizeof(float)));

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Matrix4x4 t = Matrix4x4::Translation(Vector3(0,0,-10));
        Matrix4x4 r = Matrix4x4::Rotation(45, AXIS_Y);
        r = r*Matrix4x4::Rotation(45, AXIS_X);
        Matrix4x4 m = t*r;
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, m.Transpose().Start());

        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();

        glDisableVertexAttribArray(posAttrib);
        glDisableVertexAttribArray(colAttrib);
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

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

    // Set up lighting
    GLfloat light_position[] = {1.0f,1.0f,1.0f,0.0f};
    GLfloat light_color[]={ 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ambient_color[]={ 0.4f, 0.4f, 0.4f, 1.0f };

    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position );
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color );

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(40.0, ratio, 0.1, 1000.0);
    */
    glewExperimental = GL_TRUE;
    glewInit();

    return window;
}

void Cleanup(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
