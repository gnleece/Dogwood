#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

// This define is needed to make Qt play nice with GLEW. It must come before the QGLWidget include!
#define QT_NO_OPENGL_ES_2
#include <QGLWidget>

#include "GLWidget.h"
#include "Math\Algebra.h"
#include <unordered_map>

using std::unordered_map;

class SceneViewWidget : public GLWidget
{
    Q_OBJECT

public:
    SceneViewWidget(QWidget *parent = 0);

    void update();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);

private:
    const float KEY_TRANS_AMOUNT = 2.f;
    const float KEY_ROT_AMOUNT = 35.f;
    const float MOUSE_ROT_AMOUNT = 7.f;

    void MoveCamera(Vector3 localSpaceOffset);
    void RotateCamera(eAXIS axis, float degrees);

    bool m_hasFocus;
    bool m_mousePressed;
    bool m_mouseDragging;
    
    unordered_map<int, bool> m_keyStates;

    QPoint m_prevMousePos;
};