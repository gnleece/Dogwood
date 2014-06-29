#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

// This define is needed to make Qt play nice with GLEW
#define QT_NO_OPENGL_ES_2
#include <QGLWidget>

class GameObject;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void SetRoot(GameObject* root);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    GameObject* m_root;
};