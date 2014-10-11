#include "Widgets\SceneViewWidget.h"

#include "DebugLogger.h"
#include "Rendering\RenderManager.h"
#include <QtWidgets>

SceneViewWidget::SceneViewWidget(QWidget* parent)
: GLWidget(parent), m_hasFocus(false), m_mousePressed(false), m_mouseDragging(false)
{}

void SceneViewWidget::update()
{
    GLWidget::update();

    if (m_hasFocus)
    {
        if (m_keyStates[Qt::Key::Key_W])
        {
            MoveCamera(Vector3(0, 0, KEY_TRANS_AMOUNT*0.01/**deltaTime*/));
        }
        if (m_keyStates[Qt::Key::Key_S])
        {
            MoveCamera(Vector3(0, 0, -KEY_TRANS_AMOUNT*0.01/**deltaTime*/));
        }
        if (m_keyStates[Qt::Key::Key_A])
        {
            MoveCamera(Vector3(KEY_TRANS_AMOUNT*0.01/**deltaTime*/, 0, 0));
        }
        if (m_keyStates[Qt::Key::Key_D])
        {
            MoveCamera(Vector3(-KEY_TRANS_AMOUNT*0.01/**deltaTime*/, 0, 0));
        }
    }
}

void SceneViewWidget::mousePressEvent(QMouseEvent* event)
{
    m_mousePressed = true;
}

void SceneViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (m_mouseDragging)
    {
        float deltaX = pos.x() - m_prevMousePos.x();
        float deltaY = pos.y() - m_prevMousePos.y();
        RotateCamera(AXIS_Y, deltaX*MOUSE_ROT_AMOUNT*0.01/**deltaTime*/);
        RotateCamera(AXIS_X, deltaY*MOUSE_ROT_AMOUNT*0.01/**deltaTime*/);
    }
    else
    {
        m_mouseDragging = true;
    }
    m_prevMousePos = pos;
}

void SceneViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_mousePressed = false;
    m_mouseDragging = false;
}

void SceneViewWidget::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    m_keyStates[key] = true;
}

void SceneViewWidget::keyReleaseEvent(QKeyEvent* event)
{
    int key = event->key();
    m_keyStates[key] = false;
}

void SceneViewWidget::focusInEvent(QFocusEvent* event)
{
    m_hasFocus = true;
}

void SceneViewWidget::focusOutEvent(QFocusEvent* event)
{
    m_hasFocus = false;
    m_mousePressed = false;
    m_keyStates.clear();
}

void SceneViewWidget::MoveCamera(Vector3 localSpaceOffset)
{
    Matrix4x4 view = RenderManager::Singleton().GetView();
    view = Translation(localSpaceOffset)*view;
    RenderManager::Singleton().SetView(view);
}

void SceneViewWidget::RotateCamera(eAXIS axis, float degrees)
{
    Matrix4x4 view = RenderManager::Singleton().GetView();
    view = Rotation(degrees, axis)*view;
    RenderManager::Singleton().SetView(view);
}