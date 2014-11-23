#include "Widgets\SceneViewWidget.h"

#include "DebugLogger.h"
#include "Debugging\DebugDraw.h"
#include "GameObject.h"
#include "MainEditorWindow.h"
#include "Rendering\RenderManager.h"
#include "Scene\Scene.h"
#include <QtWidgets>

SceneViewWidget::SceneViewWidget(MainEditorWindow* window, QWidget* parent)
: m_window(window), GLWidget(parent), m_hasFocus(false), m_showGrid(true)
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

void SceneViewWidget::SetScene(Scene* scene)
{
    m_scene = scene;
}


void SceneViewWidget::update()
{
    GLWidget::update();

    DebugDraw::Singleton().DrawLineBuffer(m_gridVAO, m_gridVBO, m_gridLinesVertexBuffer, GRID_BUFFER_SIZE, m_gridColor);
}

void SceneViewWidget::mousePressEvent(QMouseEvent* event)
{
    eMouseButton button = QtMouseButtonConvert(event->button());
    if (button < NUM_MOUSE_BUTTONS)
    {
        m_mousePressed[button] = true;
    }
    m_prevMousePos = event->pos();

    if (button == OBJECT_SELECT_BUTTON)
    {
        PickObject(event->localPos());
    }
}

void SceneViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    if (m_mousePressed[CAMERA_ROTATE_BUTTON])
    {
        float deltaX = pos.x() - m_prevMousePos.x();
        float deltaY = pos.y() - m_prevMousePos.y();
        RotateCamera(AXIS_Y, deltaX*CAMERA_ROTATE_AMOUNT);
        RotateCamera(AXIS_X, deltaY*CAMERA_ROTATE_AMOUNT);
    }

    if (m_mousePressed[CAMERA_PAN_BUTTON])
    {
        float deltaX = pos.x() - m_prevMousePos.x();
        float deltaY = pos.y() - m_prevMousePos.y();
        MoveCamera(Vector3(deltaX*CAMERA_PAN_AMOUNT, 0, 0));
        MoveCamera(Vector3(0, -1*deltaY*CAMERA_PAN_AMOUNT, 0));
    }

    m_prevMousePos = pos;
}

void SceneViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    eMouseButton button = QtMouseButtonConvert(event->button());
    if (button < NUM_MOUSE_BUTTONS)
    {
        m_mousePressed[button] = false;
    }
}

void SceneViewWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    if (m_hasFocus)
    {
        MoveCamera(Vector3(0, 0, delta*CAMERA_ZOOM_AMOUNT));
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
    Matrix4x4 view = RenderManager::Singleton().GetView().GetMatrix();
    view = Translation(localSpaceOffset)*view;
    RenderManager::Singleton().SetView(view);
}

void SceneViewWidget::RotateCamera(eAXIS axis, float degrees)
{
    Matrix4x4 view = RenderManager::Singleton().GetView().GetMatrix();
    view = Rotation(degrees, axis)*view;
    RenderManager::Singleton().SetView(view);
}

void SceneViewWidget::ClearMouseButtonState()
{
    for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
    {
        m_mousePressed[i] = false;
    }
}

// Based on tutorial from: http://antongerdelan.net/opengl/raycasting.html
void SceneViewWidget::PickObject(const QPointF clickPosition)
{
    if (m_scene == NULL || m_scene->GetRootObject() == NULL)
        return;

    // Viewport coords: x in [0, width], y in [0, height]
    int screenX = clickPosition.x();
    int screenY = clickPosition.y();

    // Normalized coords: x in [-1, 1], y in [-1, 1]
    int width = RenderManager::Singleton().GetConfig().width;
    int height = RenderManager::Singleton().GetConfig().height;
    float normalizedX = (2.0f * screenX) / width - 1.0f;
    float normalizedY = 1.0f - (2.0f * screenY) / height;

    // Clip coords
    Vector4 rayDirectionClipSpace = Vector4(normalizedX, normalizedY, -1.0, 1.0);

    // Camera coords
    Vector4 rayDirectionCameraSpace = RenderManager::Singleton().GetProjection().GetMatrix().Inverse() * rayDirectionClipSpace;
    rayDirectionCameraSpace[2] = -1;     // we only need to unproject x and y, not z and w
    rayDirectionCameraSpace[3] = 0;

    // World coords
    Vector3 rayDirectionWorldSpace = (RenderManager::Singleton().GetView().GetMatrix().Inverse() * rayDirectionCameraSpace).xyz();
    rayDirectionWorldSpace = rayDirectionWorldSpace.Normalized();

    // Ray origin is camera position
    Vector4 cameraPosition = Vector4(RenderManager::Singleton().GetView().GetPosition(), 0);
    Vector4 cameraPositionWorldSpace = RenderManager::Singleton().GetView().GetMatrix().Inverse() * cameraPosition;
    Vector3 rayOriginWorldSpace = -1 * cameraPositionWorldSpace.xyz(); // TODO multiply by -1 is a hack, need to fix camera/view setup properly

    // Do raycast against all objects in hierarchy   TODO this is pretty terrible
    float hitDistance;
    GameObject* hitObject = m_scene->GetRootObject()->BoundingSphereRaycast(rayOriginWorldSpace, rayDirectionWorldSpace, Transform::Identity, hitDistance);
    if (hitObject != NULL)
    {
        m_window->SelectObject(hitObject);
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