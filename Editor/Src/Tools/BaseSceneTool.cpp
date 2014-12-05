#include "Tools\BaseSceneTool.h"

void BaseSceneTool::Init(SceneViewWidget* parent)
{
    m_parent = parent;
    m_active = false;
}

bool BaseSceneTool::IsActive()
{
    return m_active;
}