#include "Widgets\SceneViewWidget.h"

#include "DebugLogger.h"
#include "Debugging\DebugDraw.h"
#include "Rendering\RenderManager.h"
#include <QtWidgets>

SceneViewWidget::SceneViewWidget(QWidget* parent)
: GLWidget(parent), m_hasFocus(false), m_showGrid(true)
{
    ClearMouseButtonState();
}

void SceneViewWidget::PostSetup()
{
    // This setup can't be done in the constructor, because glewInit hasn't been called
    // at that point, and we need opengl to be ready

    // Make a buffer of lines for a unit grid in the x-z plane
    int z = -GRID_SIZE;
    for (int i = 0; i < GRID_SIZE * 2 + 1; i++)
    {
        m_gridLinesVertexBuffer[i * 2] = Vector3(-GRID_SIZE, 0, z);
        m_gridLinesVertexBuffer[i * 2 + 1] = Vector3(GRID_SIZE, 0, z);
        z++;
    }
    int x = -GRID_SIZE;
    for (int i = 0; i < GRID_SIZE * 2 + 1; i++)
    {
        m_gridLinesVertexBuffer[(GRID_SIZE * 4 + 2) + i * 2] = Vector3(x, 0, -GRID_SIZE);
        m_gridLinesVertexBuffer[(GRID_SIZE * 4 + 2) + i * 2 + 1] = Vector3(x, 0, GRID_SIZE);
        x++;
    }

    m_gridColor = ColourRGB(0.5f, 0.5f, 0.5f);

    DebugDraw::Singleton().PrepareLineBuffer(m_gridLinesVertexBuffer, GRID_BUFFER_SIZE, m_gridVAO, m_gridVBO);
}

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

    DebugDraw::Singleton().DrawLineBuffer(m_gridVAO, m_gridVBO, m_gridLinesVertexBuffer, GRID_BUFFER_SIZE, m_gridColor);
}

void SceneViewWidget::mousePressEvent(QMouseEvent* event)
{
    eMouseButton button = QtMouseButtonConvert(event->button());
    if (button < NUM_MOUSE_BUTTONS)
    {
        m_mousePressed[button] = true;
    }
}

void SceneViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    // Middle mouse button rotates the camera
    if (m_mouseDragging[MOUSE_BUTTON_MIDDLE])
    {
        float deltaX = pos.x() - m_prevMousePos.x();
        float deltaY = pos.y() - m_prevMousePos.y();
        RotateCamera(AXIS_Y, deltaX*MOUSE_ROT_AMOUNT*0.01/**deltaTime*/);
        RotateCamera(AXIS_X, deltaY*MOUSE_ROT_AMOUNT*0.01/**deltaTime*/);
    }
    else if (m_mousePressed[MOUSE_BUTTON_MIDDLE])
    {
        m_mouseDragging[MOUSE_BUTTON_MIDDLE] = true;
    }

    m_prevMousePos = pos;
}

void SceneViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    eMouseButton button = QtMouseButtonConvert(event->button());
    if (button < NUM_MOUSE_BUTTONS)
    {
        m_mousePressed[button] = false;
        m_mouseDragging[button] = false;
    }
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
    ClearMouseButtonState();
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

void SceneViewWidget::ClearMouseButtonState()
{
    for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
    {
        m_mousePressed[i] = false;
        m_mouseDragging[i] = false;
    }
}

SceneViewWidget::eMouseButton SceneViewWidget::QtMouseButtonConvert(Qt::MouseButton qtButton)
{
    switch (qtButton)
    {
        case Qt::MouseButton::LeftButton:   return MOUSE_BUTTON_LEFT;
        case Qt::MouseButton::RightButton:  return MOUSE_BUTTON_RIGHT;
        case Qt::MouseButton::MiddleButton: return MOUSE_BUTTON_MIDDLE;
        default:                            return NUM_MOUSE_BUTTONS;
    }
}