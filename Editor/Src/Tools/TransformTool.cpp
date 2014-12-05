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

bool TransformTool::OnMouseDown(int screenX, int screenY, Vector3 rayOrigin, Vector3 rayDirection)
{
    float arrowRadius = m_arrowHeight/2;

    // Raycast against each arrow of the gnomon
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
        DebugLogger::Singleton().Log("Transform Tool ON");
        m_active = true;
        m_activeAxis = (eAXIS)arrowIndex;

        m_point0 = RenderManager::Singleton().ToScreenSpace(m_transform.GetPosition());
        m_point1 = RenderManager::Singleton().ToScreenSpace((m_transform*m_arrowTransforms[m_activeAxis]).GetPosition());

        float a = (m_point1.y() - m_point0.y()) / (m_point1.x() - m_point0.x());
        float b = -1;
        float c = m_point0.y() - a*m_point0.x();

        // Find the closet point on the screen space line to the mouse click position
        m_prevLineX = (screenX + a*screenY - a*c) / (a*a + 1);
        m_prevLineY = (a*(screenX + a*screenY) + c) / (a*a + 1);
        Vector2 A = Vector2(m_prevLineX, m_prevLineY) - m_point0;
        Vector2 B = m_point1 - m_point0;
        float t = A.x() / B.x();
        return true;
    }

    m_active = false;
    return false;
}

void TransformTool::OnMouseMove(int screenX, int screenY)
{
    if (m_active)
    {
        // Convert active axis line from world space to screen space
        float a = (float)(m_point1.y() - m_point0.y()) / (float)(m_point1.x() - m_point0.x());
        float b = -1;
        float c = m_point0.y() - a*m_point0.x();

        // Find the closet point on the screen space line to the mouse click position
        float x = (screenX + a*screenY - a*c) / (a*a + 1);
        float y = (a*(screenX + a*screenY) + c) / (a*a + 1);


        // Use the delta to adjust the select object position
        Vector3 offset;
        float scale = 0.1f;
        Vector2 A = Vector2(x, y) - m_point0;
        Vector2 B = m_point1 - m_point0;
        float t = A.x() / B.x();
        if (t > m_prevT)
        {
            // positive motion
            offset = scale*m_arrowTransforms[m_activeAxis].GetPosition();
            DebugLogger::Singleton().Log("positive");
        }
        else
        {
            // negative motion
            offset = -1*scale*m_arrowTransforms[m_activeAxis].GetPosition();
            DebugLogger::Singleton().Log("negative");
        }
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