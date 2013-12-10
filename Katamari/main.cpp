// main.cpp : Defines the entry point for the console application.
//

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <ctime>
#include <stdlib.h>
#include <stdio.h>

#include "Engine\Algebra.h"
#include "Engine\Primitive.h"
#include "Engine\Util.h"

GLFWwindow* Setup();
void Cleanup(GLFWwindow* window);
void Render(GLFWwindow* window);

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

    // Open.gl tutorial setup
    float vertices[] = {
         0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
    };

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);                  // Generate buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);     // Make buffer active
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy data to buffer

    GLuint vertexShader = loadShaderFromFile("VertexTest.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShaderFromFile("FragmentTest.glsl", GL_FRAGMENT_SHADER);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    Matrix4x4 m = Matrix4x4::Rotation(180, AXIS_Z);
    GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, m.Start());

    while (!glfwWindowShouldClose(window))
    {

        // Specify the layout of the vertex data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

        GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                               5*sizeof(float), (void*)(2*sizeof(float)));

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(40.0, ratio, 0.1, 1000.0);

    glewExperimental = GL_TRUE;
    glewInit();

    return window;
}

void Cleanup(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Render(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    // Set the initial camera
    glTranslated(0.0, 0, -5.0);
    glRotated(10,1.0,0.0,0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Position the camera
    float m_zoom = 1.0f;
    glScaled(m_zoom,m_zoom,m_zoom);
    //glMultMatrixd(m_camera.transpose().begin());

    // Position the light source
    GLfloat light_position[] = {1.0,1.0,1.0,0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position );

    // DRAW SCENE
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Cleanup
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glEnd();
}