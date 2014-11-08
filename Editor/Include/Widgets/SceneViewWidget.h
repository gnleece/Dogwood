#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "GLWidget.h"
#include "Math\Algebra.h"
#include "Rendering\Colour.h"
#include <unordered_map>

using std::unordered_map;

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

    SceneViewWidget(QWidget *parent = 0);

    void PostSetup();
    void update();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);

private:
    const float KEY_TRANS_AMOUNT = 6.f;
    const float KEY_ROT_AMOUNT = 35.f;
    const float MOUSE_ROT_AMOUNT = 7.f;

    const static int GRID_SIZE = 10;
    const static int GRID_BUFFER_SIZE = (GRID_SIZE * 2 + 1) * 2 * 2; // GRID_SIZE * (pos + neg) + zero * (horiz + vert) * (points per line)

    void MoveCamera(Vector3 localSpaceOffset);
    void RotateCamera(eAXIS axis, float degrees);
    void ClearMouseButtonState();
    static eMouseButton QtMouseButtonConvert(Qt::MouseButton qtButton);

    bool                        m_hasFocus;
    bool                        m_mousePressed[NUM_MOUSE_BUTTONS];
    bool                        m_mouseDragging[NUM_MOUSE_BUTTONS];
    
    unordered_map<int, bool>    m_keyStates;
    QPoint                      m_prevMousePos;

    // Unit grid (x-z plane)
    bool                        m_showGrid;
    ColourRGB                   m_gridColor;
    GLuint                      m_gridVAO;
    GLuint                      m_gridVBO;
    Vector3                     m_gridLinesVertexBuffer[GRID_BUFFER_SIZE];
};