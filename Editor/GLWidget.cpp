#include "GLWidget.h"

#include <qcolor.h>
#include <QtOpenGL>
#include <QtWidgets>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{}

GLWidget::~GLWidget()
{}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
    QColor clearColor = QColor::fromCmykF(1.0, 0.0, 0.0, 0.0);
    qglClearColor(clearColor.dark());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::resizeGL(int width, int height)
{}

void GLWidget::mousePressEvent(QMouseEvent *event)
{}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{}