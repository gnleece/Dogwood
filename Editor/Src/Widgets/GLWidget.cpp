#include "Widgets\GLWidget.h"

#include <qcolor.h>
#include <QtOpenGL>
#include <QtWidgets>

#include "DebugLogger.h"
#include "Rendering\RenderManager.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

GLWidget::~GLWidget()
{}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(640, 480);
}

QSize GLWidget::maximumSizeHint() const
{
    return QSize(640, 480);
}

QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::initializeGL()
{}

void GLWidget::paintGL()
{}

void GLWidget::resizeGL(int /*width*/, int /*height*/)
{
    // TODO implement me
}