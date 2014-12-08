#include "Widgets\SceneViewWidget.h"

#include "DebugLogger.h"
#include "Debugging\DebugDraw.h"
#include "EditorCommands.h"
#include "GameObject.h"
#include "MainEditorWindow.h"
#include "Rendering\RenderManager.h"
#include "Scene\Scene.h"
#include <QtWidgets>

#include "Math\Transformations.h"

SceneViewWidget::SceneViewWidget(MainEditorWindow* window, QWidget* parent)
: m_window(window), GLWidget(parent), m_hasFocus(false), m_showGrid(true), m_scene(NULL),
  m_cameraPitch(0), m_cameraYaw(0), m_activeTool(NULL)
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
    m_transformTool.Init(this);
}

void SceneViewWidget::SetScene(Scene* scene)
{
    m_scene = scene;
}

void SceneViewWidget::update()
{
    GLWidget::update();

    DebugDraw::Singleton().DrawLineBuffer(m_gridVAO, m_gridVBO, m_gridLinesVertexBuffer, GRID_BUFFER_SIZE, m_gridColor);

    GameObject* selectedObject = m_window->GetSelectedObject();
    if (selectedObject)
    {
        m_transformTool.Draw(selectedObject->GetWorldTransform());
    }
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
        HandleSelectionClick(event->localPos());
    }
}

void SceneViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    if (m_mousePressed[CAMERA_ROTATE_BUTTON])
    {
        float deltaX = pos.x() - m_prevMousePos.x();
        float deltaY = pos.y() - m_prevMousePos.y();
        RotateCamera(DEBUG_CAMERA_YAW, deltaX*CAMERA_ROTATE_AMOUNT);
        RotateCamera(DEBUG_CAMERA_PITCH, deltaY*CAMERA_ROTATE_AMOUNT);
    }

    if (m_mousePressed[CAMERA_PAN_BUTTON])
    {
        float deltaX = pos.x() - m_prevMousePos.x();
        float deltaY = pos.y() - m_prevMousePos.y();
        TranslateCamera(Vector3(deltaX*CAMERA_PAN_AMOUNT, 0, 0));
        TranslateCamera(Vector3(0, -1 * deltaY*CAMERA_PAN_AMOUNT, 0));
    }

    m_prevMousePos = pos;

    if (m_activeTool != NULL)
    {
        m_activeTool->OnMouseMove(pos.x(), pos.y());
    }
}

void SceneViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    eMouseButton button = QtMouseButtonConvert(event->button());
    if (button < NUM_MOUSE_BUTTONS)
    {
        m_mousePressed[button] = false;
    }

    if (button == OBJECT_SELECT_BUTTON)
    {
        if (m_activeTool != NULL)
        {
            m_activeTool->OnMouseUp();
        }
    }
}

void SceneViewWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    if (m_hasFocus)
    {
        TranslateCamera(Vector3(0, 0, delta*CAMERA_ZOOM_AMOUNT));
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
    if (m_activeTool)
    {
        m_activeTool->OnMouseUp();
    }
}

void SceneViewWidget::MoveSelectedObject(Vector3 offset)
{
    Vector3 curPos = m_window->GetSelectedObject()->GetLocalTransform().GetPosition();
    Vector3 newPos = curPos + offset;
    m_window->UpdateGameObjectTransform(newPos, eVector_Position);
}

void SceneViewWidget::TranslateCamera(Vector3 localSpaceOffset)
{
    // Convert the local (camera) space offset to world coords
    Matrix4x4 cameraRotation = Rotation(m_cameraPitch, eAXIS::AXIS_X)*Rotation(m_cameraYaw, eAXIS::AXIS_Y);
    Vector3 offset = (Vector4(localSpaceOffset, 0)*cameraRotation).xyz();

    // Add new offset to total offset and re-set view matrix
    m_cameraOffset = m_cameraOffset + offset;
    SetViewMatrix();
}

// Camera only rotates in world X and world Y (i.e. only pitch and yaw, no roll)
void SceneViewWidget::RotateCamera(CameraRotationType type, float degrees)
{
    if (type == DEBUG_CAMERA_PITCH)
    {
        m_cameraPitch += degrees;
    }
    else if (type == DEBUG_CAMERA_YAW)
    {
        m_cameraYaw += degrees;
    }
    SetViewMatrix();
}

void SceneViewWidget::SetViewMatrix()
{
    // We accumulate pitch and yaw separately because we want to rotate the camera in
    // world coords and never rotate in z (roll)
    Matrix4x4 view = Rotation(m_cameraPitch, eAXIS::AXIS_X)*Rotation(m_cameraYaw, eAXIS::AXIS_Y);

    view = view*Translation(m_cameraOffset);
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
void SceneViewWidget::HandleSelectionClick(const QPointF clickPosition)
{
    if (m_scene == NULL || m_scene->GetRootObject() == NULL)
        return;

    // TODO move this code into a Util function somewhere

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

    // First, check whether the click hit any of the tools
    bool hitTool = PickTool(clickPosition, rayOriginWorldSpace, rayDirectionWorldSpace);

    // If not, check whether the click hit any gameobjects
    if (!hitTool)
    {
        PickObject(rayOriginWorldSpace, rayDirectionWorldSpace);
    }
}

bool SceneViewWidget::PickTool(const QPointF clickPosition, Vector3 rayOrigin, Vector3 rayDirection)
{
    if (m_transformTool.OnMouseDown(clickPosition.x(), clickPosition.y(), rayOrigin, rayDirection))
    {
        m_activeTool = &m_transformTool;
        return true;
    }
    return false;
}

bool SceneViewWidget::PickObject(Vector3 rayOrigin, Vector3 rayDirection)
{
    // Do raycast against all objects in hierarchy   TODO this is pretty terrible
    float hitDistance;
    GameObject* hitObject = m_scene->GetRootObject()->BoundingSphereRaycast(rayOrigin, rayDirection, Transform::Identity, hitDistance);
    if (hitObject != NULL)
    {
        m_window->SelectObject(hitObject);
        return true;
    }
    m_window->SelectObject(NULL);
    return false;
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