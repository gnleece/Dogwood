#include "DebugLogger.h"
#include "Math\Raycast.h"
#include "Math\Transformations.h"
#include "Tools\TransformTool.h"


void TransformTool::Init()
{
    m_arrowBase = 0.1f;
    m_arrowHeight = 0.2f;
    m_gnomon.Init(m_arrowBase, m_arrowHeight);

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

bool TransformTool::OnClick(Vector3 rayOrigin, Vector3 rayDirection)
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
        return true;
    }

    return false;
}