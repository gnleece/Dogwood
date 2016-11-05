#include "Widgets\SceneViewWidget.h"

#include "DebugLogger.h"
#include "MainEditorWindow.h"
#include "ToolsideGameObject.h"
#include "Debugging\DebugDraw.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\RenderManager.h"
#include "Scene\Scene.h"
#include <QtWidgets>

#include "Math\Transformations.h"
#include "Math\Raycast.h"

using namespace EditorCommands;

SceneViewWidget::SceneViewWidget(MainEditorWindow* window, QWidget* parent)
: m_window(window), GLWidget(parent), m_hasFocus(false), m_showGrid(true), m_scene(NULL),
  m_cameraPitch(0), m_cameraYaw(0), m_activeTool(NULL)
{
    ClearMouseButtonState();
}

void SceneViewWidget::PostSetup()
{
    // This setup can't be done in the constructor, because glewInit hasn't been called
    // at that point, and we need OpenGL to be ready

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

    m_gridColor = ColorRGB(0.5f, 0.5f, 0.5f);

    DebugDraw::Singleton().PrepareLineBuffer(m_gridLinesVertexBuffer, GRID_BUFFER_SIZE, m_gridVAO, m_gridVBO);
    m_transformTool.Init(this);
}

void SceneViewWidget::SetScene(Scene* scene)
{
    m_scene = scene;

    // TODO determine startup camera position / rotation
    m_cameraYaw = 180;
}

void SceneViewWidget::update()
{
    GLWidget::update();

    DebugDraw::Singleton().DrawLineBuffer(m_gridVAO, m_gridVBO, m_gridLinesVertexBuffer, GRID_BUFFER_SIZE, m_gridColor);

    ToolsideGameObject* selectedObject = m_window->GetSelectedObject();
    if (selectedObject)
    {
        m_transformTool.SetTargetTransform(&(selectedObject->GetTransform()));
        m_transformTool.Draw();
    }
    else
    {
        m_transformTool.SetTargetTransform(NULL);
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
        TranslateCamera(Vector3(-deltaX*CAMERA_PAN_AMOUNT, 0, 0));
        TranslateCamera(Vector3(0, deltaY*CAMERA_PAN_AMOUNT, 0));
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
        TranslateCamera(Vector3(0, 0, -delta*CAMERA_ZOOM_AMOUNT));
    }
}

void SceneViewWidget::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    m_keyStates[key] = true;

    // TODO temp hack, remove
    switch(key)
    {
    case 16777234: TranslateCamera(Vector3(CAMERA_PAN_AMOUNT * -5, 0, 0)); break;  // Left
    case 16777236: TranslateCamera(Vector3(CAMERA_PAN_AMOUNT * 5, 0, 0));  break;  // Right
    case 16777235: TranslateCamera(Vector3(0, CAMERA_PAN_AMOUNT * 5, 0));  break;  // Up
    case 16777237: TranslateCamera(Vector3(0, CAMERA_PAN_AMOUNT * -5, 0)); break;  // Down
    }
}

void SceneViewWidget::keyReleaseEvent(QKeyEvent* event)
{
    int key = event->key();
    m_keyStates[key] = false;
}

void SceneViewWidget::focusInEvent(QFocusEvent* /*event*/)
{
    m_hasFocus = true;
}

void SceneViewWidget::focusOutEvent(QFocusEvent* /*event*/)
{
    m_hasFocus = false;
    ClearMouseButtonState();
    m_keyStates.clear();
    if (m_activeTool)
    {
        m_activeTool->OnMouseUp();
    }
}

void SceneViewWidget::TranslateSelectedObject(Vector3& offset)
{
    Vector3 position = m_window->GetSelectedObject()->GetTransform().GetLocalPosition();
    position = position + offset;
    ExecuteModifyTransform(position, eVector_Position);
}

void SceneViewWidget::RotateSelectedObject(float offset, eAXIS axis)
{
    Vector3 rotation = m_window->GetSelectedObject()->GetTransform().GetLocalRotation();
    rotation[axis] = rotation[axis] + offset;
    ExecuteModifyTransform(rotation, eVector_Rotation);
}

void SceneViewWidget::ScaleSelectedObject(float offset, eAXIS axis)
{
    Vector3 scale = m_window->GetSelectedObject()->GetTransform().GetLocalScale();
    scale[axis] = Clamp(scale[axis] + offset, 0, FLT_MAX);
    ExecuteModifyTransform(scale, eVector_Scale);
}

void SceneViewWidget::SetTransformToolMode(TransformTool::eMode mode)
{
    m_transformTool.SetMode(mode);
}

void SceneViewWidget::SetTransformToolSpace(TransformTool::eSpace space)
{
    m_transformTool.SetSpace(space);
}

void SceneViewWidget::TranslateCamera(Vector3& localSpaceOffset)
{
    Matrix4x4 cameraRotation = Rotation(m_cameraPitch, eAXIS::AXIS_X)*Rotation(m_cameraYaw, eAXIS::AXIS_Y);
    Vector3 offset = (Vector4(localSpaceOffset, 0)*cameraRotation).xyz();

    Vector3 newCameraPosition = RenderManager::Singleton().GetCamera().GetPosition() + offset;
    RenderManager::Singleton().GetCamera().GetCameraTransform().SetLocalPosition(newCameraPosition);
}

void SceneViewWidget::RotateCamera(CameraRotationType type, float degrees)
{
    // We accumulate pitch and yaw separately because we want to rotate the camera in
    // world coords and never rotate in z (roll)
    if (type == DEBUG_CAMERA_PITCH)
    {
        m_cameraPitch += degrees;
    }
    else if (type == DEBUG_CAMERA_YAW)
    {
        m_cameraYaw += degrees;
    }

    Matrix4x4 view = Rotation(m_cameraPitch, eAXIS::AXIS_X)*Rotation(m_cameraYaw, eAXIS::AXIS_Y);
    view = view*Translation(RenderManager::Singleton().GetCamera().GetPosition());
    RenderManager::Singleton().GetCamera().SetViewTransform(Transform(view));
}

void SceneViewWidget::CenterCameraOnSelectedObject()
{
    ToolsideGameObject* selectedObject = m_window->GetSelectedObject();
    
    if (selectedObject == NULL)
        return;

    // Calculate how far away from the selected object to place the camera
    float distance = 4.0f;
    if (selectedObject->GetMeshInstance())
    {
        float size = selectedObject->GetMeshInstance()->GetBoundingRadius();
        distance = distance * size;
    }

    // Position the camera so that the selected object is in the center of the view.
    Vector3 objectPosition = selectedObject->GetTransform().GetWorldPosition();
    Vector3 cameraForward = RenderManager::Singleton().GetCamera().GetCameraTransform().GetForward();
    Vector3 newCameraPosition = objectPosition + distance*cameraForward;

    RenderManager::Singleton().GetCamera().GetCameraTransform().SetLocalPosition(newCameraPosition);

    // TODO this doesn't account for other objects that might be in the way in the line of sight
    // TODO lerp to new camera position instead of jumping instantly
    // TODO camera distance is weird for some objects, debug it
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

    // TODO move this code into Camera class

    // Viewport coords: x in [0, width], y in [0, height]
    int screenX = clickPosition.x();
    int screenY = clickPosition.y();

    // Normalized coords: x in [-1, 1], y in [-1, 1]
    int width = RenderManager::Singleton().GetViewportWidth();
    int height = RenderManager::Singleton().GetViewportHeight();
    float normalizedX = (2.0f * screenX) / width - 1.0f;
    float normalizedY = 1.0f - (2.0f * screenY) / height;

    // Ray direction: clip space
    Vector4 rayDirectionClipSpace = Vector4(normalizedX, normalizedY, -1.0, 1.0);

    // Ray direction: clip space -> camera space
    Vector4 rayDirectionCameraSpace = RenderManager::Singleton().GetCamera().GetProjectionTransform().GetWorldMatrix().Inverse() * rayDirectionClipSpace;
    rayDirectionCameraSpace[2] = -1;     // we only need to unproject x and y, not z and w
    rayDirectionCameraSpace[3] = 0;

    // Ray direction: camera space -> world space
    Vector3 rayDirectionWorldSpace = (RenderManager::Singleton().GetCamera().GetCameraTransform() * rayDirectionCameraSpace).xyz();
    rayDirectionWorldSpace = rayDirectionWorldSpace.Normalized();

    // Ray origin: camera position (world space)
    Vector3 rayOriginWorldSpace = RenderManager::Singleton().GetCamera().GetCameraTransform().GetWorldPosition();

    // First, check whether the click hit any of the tools
    bool hitTool = PickTool(clickPosition, rayOriginWorldSpace, rayDirectionWorldSpace);

    // If not, check whether the click hit any game objects
    if (!hitTool)
    {
        PickObject(rayOriginWorldSpace, rayDirectionWorldSpace);
    }
}

bool SceneViewWidget::PickTool(const QPointF clickPosition, Vector3& rayOrigin, Vector3& rayDirection)
{
    if (m_transformTool.OnMouseDown(clickPosition.x(), clickPosition.y(), rayOrigin, rayDirection))
    {
        m_activeTool = &m_transformTool;
        return true;
    }
    return false;
}

bool SceneViewWidget::PickObject(Vector3& rayOrigin, Vector3& rayDirection)
{
    // Raycast against the scene root to see if we have hit any objects in the scene
    GameObjectBase* rootObject = m_scene->GetToolsideRootObject();
    GameObjectBase* hitObject = NULL;
    Raycast::HitInfo hitInfo;
    Raycast::RaycastGameObject(rayOrigin, rayDirection, rootObject, true, hitObject, hitInfo);
    if (hitObject != NULL)
    {
        m_window->SelectObject((ToolsideGameObject*)hitObject);
        bool isInView = RenderManager::Singleton().GetCamera().IsInView(hitObject->GetTransform().GetWorldPosition());
        if (!isInView)
        {
            CenterCameraOnSelectedObject();
        }
        return true;
    }
    m_window->SelectObject(NULL);
    return false;
}

void SceneViewWidget::ExecuteModifyTransform(Vector3& vector, TransformVectorType type)
{
    ToolsideGameObject* selectedObject = m_window->GetSelectedObject();
    ModifyTransformCommand* command = new ModifyTransformCommand(selectedObject, vector, type);
    CommandManager::Singleton().ExecuteCommand(command);
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