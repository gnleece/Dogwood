#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

// This define is needed to make Qt play nice with GLEW. It must come before the QGLWidget include!
#define QT_NO_OPENGL_ES_2
#include <QGLWidget>

class GameObject;
class MainEditorWindow;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize maximumSizeHint() const;
    QSize sizeHint() const;

//protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    GameObject* m_root;
};