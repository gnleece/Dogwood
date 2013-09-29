// main.cpp : Defines the entry point for the console application.
//

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

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
    
    while (!glfwWindowShouldClose(window))
    {
        Render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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

    // GLFW TEST CODE:
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);

    // test sphere
    GLUquadricObj* quad;
    quad = gluNewQuadric();
    gluSphere(quad, 0.3, 20, 20);
    gluDeleteQuadric(quad);

    // Cleanup
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glEnd();
}