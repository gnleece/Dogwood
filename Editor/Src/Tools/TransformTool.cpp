#include "DebugLogger.h"
#include "Math\Raycast.h"
#include "Math\Transformations.h"
#include "Rendering\RenderManager.h"
#include "Tools\TransformTool.h"
#include "Widgets\SceneViewWidget.h"

void TransformTool::Init(SceneViewWidget* parent)
{
    BaseSceneTool::Init(parent);
    m_mode = TOOL_MODE_TRANSLATE;
    m_transform = NULL;
    
    // Init gnomon
    m_arrowBase = 0.25f;
    m_arrowHeight = 0.5f;
    m_gnomon.Init(m_arrowBase, m_arrowHeight);

    // Calculate & store the transforms for each arrow on the gnomon
    float offset = m_arrowHeight / 2;
    m_arrowTransforms[0] = Transform(Translation(Vector3(1 + offset, 0, 0)));
    m_arrowTransforms[1] = Transform(Translation(Vector3(0, 1 + offset, 0)));
    m_arrowTransforms[2] = Transform(Translation(Vector3(0, 0, 1 + offset)));
}

void TransformTool::SetTargetTransform(Transform* transform)
{
    m_transform = transform;
}

void TransformTool::Draw()
{
    if (m_transform != NULL)
    {
        m_gnomon.Draw(*m_transform);
    }
}

void TransformTool::SetMode(eMode mode)
{
    m_mode = mode;
}

TransformTool::eMode TransformTool::GetMode()
{
    return m_mode;
}

bool TransformTool::OnMouseDown(int screenX, int screenY, Vector3 rayOrigin, Vector3 rayDirection)
{
    if (m_transform == NULL)
        return false;

    Transform gnomonTransform = m_gnomon.GetScaledTransform(*m_transform);
    float arrowRadius = m_arrowHeight/2;

    // Raycast against each arrow of the gnomon to determine if one was clicked
    float minDistance = FLT_MAX;
    int arrowIndex = -1;
    for (int i = 0; i < 3; i++)
    {
        Raycast::HitInfo hitInfo;
        Vector3 arrowCenter = ((gnomonTransform)*m_arrowTransforms[i]).GetWorldPosition();
        bool hit = Raycast::RaycastSphere(rayOrigin, rayDirection, arrowRadius, arrowCenter, hitInfo);
        if (hit && hitInfo.distance < minDistance)
        {
            minDistance = hitInfo.distance;
            arrowIndex = i;
        }
    }

    if (arrowIndex != -1)
    {
        // Activate the selected arrow
        m_activeAxis = (eAXIS)arrowIndex;

        // Calculate the equation of the line for the active axis, in screen space
        m_activeAxisPoint0 = RenderManager::Singleton().ToScreenSpace(gnomonTransform.GetWorldPosition());
        m_activeAxisPoint1 = RenderManager::Singleton().ToScreenSpace(((gnomonTransform)*m_arrowTransforms[m_activeAxis]).GetWorldPosition());
        m_vertical = true;
        if (m_activeAxisPoint1.x() - m_activeAxisPoint0.x() != 0)
        {
            // x-delta is non-zero, so the line is not vertical
            m_a = (m_activeAxisPoint1.y() - m_activeAxisPoint0.y()) / (m_activeAxisPoint1.x() - m_activeAxisPoint0.x());
            m_vertical = false;
        }
        m_c = m_activeAxisPoint0.y() - m_a*m_activeAxisPoint0.x();
        m_prevT = CalculateT(screenX, screenY);

        m_active = true;
        return true;
    }

    m_active = false;
    return false;
}

void TransformTool::OnMouseMove(int screenX, int screenY)
{
    // The line of the active axis is represented as a parametric line equation in screen space.
    // We find the point on the line that's closest to the current click position, and calculate
    // its t value. If it's greater than the previous t value, the mouse has been dragged "forward"
    // relative to the axis line, so we move the tool forward (and vice versa if the t value is less)

    if (m_active)
    {
        float scale = 0.1f;

        // Use t values to determine sign of motion
        float t = CalculateT(screenX, screenY);
        float direction = (t > m_prevT) ? 1 : -1;

        switch (m_mode)
        {
            case TOOL_MODE_TRANSLATE:   ApplyTranslation(direction, scale); break;
            case TOOL_MODE_ROTATE:      ApplyRotation(direction, scale);    break;
            case TOOL_MODE_SCALE:       ApplyScale(direction, scale);       break;
        }

        m_prevT = t;
    }
}

void TransformTool::OnMouseUp()
{
    m_active = false;
}

float TransformTool::CalculateT(float screenX, float screenY)
{
    // Find the point on the active axis line that's closet to the mouse click position,
    // and return the t value of that point
    float t = 0;
    if (m_vertical)
    {
        float A = screenY - m_activeAxisPoint0.y();
        float B = m_activeAxisPoint1.y() - m_activeAxisPoint0.y();  // guaranteed non-zero since line is vertical
        t = A / B;
    }
    else
    {
        float x = (screenX + m_a*screenY - m_a*m_c) / (m_a*m_a + 1);
        float y = (m_a*(screenX + m_a*screenY) + m_c) / (m_a*m_a + 1);
        Vector2 A = Vector2(x, y) - m_activeAxisPoint0;
        Vector2 B = m_activeAxisPoint1 - m_activeAxisPoint0;
        t = A.x() / B.x();        // B.x is guaranteed non-zero in this case (i.e. non-vertical line => non-zero x delta)
    }

    return t;
}

void TransformTool::ApplyTranslation(float direction, float scale)
{
    // Use active axis to determine direction of motion
    Vector3 offset = direction*scale*m_arrowTransforms[m_activeAxis].GetWorldPosition();

    // Convert offset into object's local space
    //offset = (m_localTransform.GetMatrix()*Vector4(offset, 0)).xyz();

    // Apply the offset
    m_parent->TranslateSelectedObject(offset);
}

void TransformTool::ApplyRotation(float direction, float scale)
{
    float offset = direction*scale*5;
    m_parent->RotateSelectedObject(offset, m_activeAxis);
}

void TransformTool::ApplyScale(float direction, float scale)
{
    float offset = direction*scale;
    m_parent->ScaleSelectedObject(offset, m_activeAxis);
}