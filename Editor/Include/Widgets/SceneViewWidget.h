#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "EditorCommands.h"
#include "GLWidget.h"
#include "Math\Algebra.h"
#include "Rendering\Color.h"
#include "Tools\BaseSceneTool.h"
#include "Tools\TransformTool.h"
#include <unordered_map>

using std::unordered_map;

class MainEditorWindow;
class Scene;

class SceneViewWidget : public GLWidget
{
    Q_OBJECT

public:
    enum eMouseButton
    {
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE,
        NUM_MOUSE_BUTTONS
    };

    SceneViewWidget(MainEditorWindow* window, QWidget *parent = 0);

    void PostSetup();
    void SetScene(Scene* scene);
    void update();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);

    void TranslateSelectedObject(Vector3& offset);
    void RotateSelectedObject(float offset, eAXIS axis);
    void ScaleSelectedObject(float offset, eAXIS axis);

    void SetTransformToolMode(TransformTool::eMode mode);
    void SetTransformToolSpace(TransformTool::eSpace space);

private:
    const float CAMERA_ROTATE_AMOUNT = 0.1f;
    const float CAMERA_PAN_AMOUNT = 0.01f;
    const float CAMERA_ZOOM_AMOUNT = 0.01f;

    const static int GRID_SIZE = 10;
    const static int GRID_BUFFER_SIZE = (GRID_SIZE * 2 + 1) * 2 * 2; // GRID_SIZE * (pos + neg) + zero * (horiz + vert) * (points per line)

    const eMouseButton OBJECT_SELECT_BUTTON = MOUSE_BUTTON_LEFT;
    const eMouseButton CAMERA_ROTATE_BUTTON = MOUSE_BUTTON_RIGHT;
    const eMouseButton CAMERA_PAN_BUTTON = MOUSE_BUTTON_MIDDLE;

    enum CameraRotationType { DEBUG_CAMERA_PITCH, DEBUG_CAMERA_YAW }; // no roll
    void TranslateCamera(Vector3& localSpaceOffset);
    void RotateCamera(CameraRotationType type, float degrees);
    void SetViewMatrix();

    void ClearMouseButtonState();

    void HandleSelectionClick(const QPointF clickPosition);
    bool PickTool(const QPointF clickPosition, Vector3& rayOrigin, Vector3& rayDirection);
    bool PickObject(Vector3& rayOrigin, Vector3& rayDirection);
    void ExecuteModifyTransform(Vector3& vector, TransformVectorType type);

    static eMouseButton QtMouseButtonConvert(Qt::MouseButton qtButton);

    MainEditorWindow*           m_window;
    Scene*                      m_scene;

    // Mouse/keyboard state
    bool                        m_hasFocus;
    bool                        m_mousePressed[NUM_MOUSE_BUTTONS];
    unordered_map<int, bool>    m_keyStates;
    QPoint                      m_prevMousePos;

    // Camera state
    float                       m_cameraPitch;
    float                       m_cameraYaw;
    Vector3                     m_cameraOffset;

    // Tools
    TransformTool               m_transformTool;
    BaseSceneTool*              m_activeTool;

    // Unit grid (x-z plane)
    bool                        m_showGrid;
    ColorRGB                    m_gridColor;
    GLuint                      m_gridVAO;
    GLuint                      m_gridVBO;
    Vector3                     m_gridLinesVertexBuffer[GRID_BUFFER_SIZE];
};