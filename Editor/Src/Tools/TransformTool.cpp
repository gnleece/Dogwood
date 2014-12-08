#include "DebugLogger.h"
#include "Math\Raycast.h"
#include "Math\Transformations.h"
#include "Rendering\RenderManager.h"
#include "Tools\TransformTool.h"
#include "Widgets\SceneViewWidget.h"

void TransformTool::Init(SceneViewWidget* parent)
{
    BaseSceneTool::Init(parent);
    
    // Init gnomon
    m_arrowBase = 0.1f;
    m_arrowHeight = 0.2f;
    m_gnomon.Init(m_arrowBase, m_arrowHeight);

    // Calculate & store the transforms for each arrow on the gnomon
    float offset = m_arrowHeight / 2;
    m_arrowTransforms[0] = Transform(Translation(Vector3(1 + offset, 0, 0)));
    m_arrowTransforms[1] = Transform(Translation(Vector3(0, 1 + offset, 0)));
    m_arrowTransforms[2] = Transform(Translation(Vector3(0, 0, 1 + offset)));
}

void TransformTool::Draw(Transform& transform)
{
    m_transform = transform;
    m_transform.SetScale(Vector3::One);
    m_gnomon.Draw(m_transform.GetMatrix());
}

void TransformTool::SetLocalTransform(Transform& transform)
{
    m_localTransform = transform;
}

bool TransformTool::OnMouseDown(int screenX, int screenY, Vector3 rayOrigin, Vector3 rayDirection)
{
    float arrowRadius = m_arrowHeight/2;

    // Raycast against each arrow of the gnomon to determine if one was clicked
    float minDistance = FLT_MAX;
    int arrowIndex = -1;
    for (int i = 0; i < 3; i++)
    {
        Raycast::HitInfo hitInfo;
        Vector3 arrowCenter = (m_transform*m_arrowTransforms[i]).GetPosition();
        bool hit = Raycast::RaycastBoundingSphere(rayOrigin, rayDirection, arrowRadius, arrowCenter, hitInfo);
        if (hit && hitInfo.distance < minDistance)
        {
            minDistance = hitInfo.distance;
            arrowIndex = i;
        }
    }

    if (arrowIndex != -1)
    {
        // Activate the selected arrow
        DebugLogger::Singleton().Log("Transform Tool ON");
        m_activeAxis = (eAXIS)arrowIndex;

        // Calculate the equation of the line for the active axis, in screen space
        m_activeAxisPoint0 = RenderManager::Singleton().ToScreenSpace(m_transform.GetPosition());
        m_activeAxisPoint1 = RenderManager::Singleton().ToScreenSpace((m_transform*m_arrowTransforms[m_activeAxis]).GetPosition());
        m_a = (m_activeAxisPoint1.y() - m_activeAxisPoint0.y()) / (m_activeAxisPoint1.x() - m_activeAxisPoint0.x());
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
    // The line of the active axis is represented as a parametic line equation in screen space.
    // We find the point on the line that's closest to the current click position, and calculte
    // its t value. If it's greater than the previous t value, the mouse has been dragged "forward"
    // relative to the axis line, so we move the tool forward (and vice versa if the t value is less)

    if (m_active)
    {
        float scale = 0.1f;

        // Use t values to determine sign of motion
        float t = CalculateT(screenX, screenY);
        float direction = (t > m_prevT) ? 1 : -1;

        // Use active axis to determine deterction of motion
        Vector3 offset = direction*scale*m_arrowTransforms[m_activeAxis].GetPosition();

        // Convert offset into object's local space
        offset = (m_localTransform.GetMatrix()*Vector4(offset, 0)).xyz();

        // Apply the offset
        m_parent->MoveSelectedObject(offset);

        m_prevT = t;
    }
}

void TransformTool::OnMouseUp()
{
    if (m_active)
    {
        DebugLogger::Singleton().Log("Transform Tool OFF");
    }
    m_active = false;
}

float TransformTool::CalculateT(float screenX, float screenY)
{
    // Find the point on the active axis line that's closet to the mouse click position,
    // and return the t value of that point
    float x = (screenX + m_a*screenY - m_a*m_c) / (m_a*m_a + 1);
    float y = (m_a*(screenX + m_a*screenY) + m_c) / (m_a*m_a + 1);
    Vector2 A = Vector2(x, y) - m_activeAxisPoint0;
    Vector2 B = m_activeAxisPoint1 - m_activeAxisPoint0;        // TODO check for division by zero
    float t = A.x() / B.x();

    return t;
}