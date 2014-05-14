#include "TestComponent.h"

#include "..\Engine\Math\Transformations.h"

void TestComponent::Update(float deltaTime)
{    
    Matrix4x4 trans = m_gameObject->GetLocalTransform();
    Matrix4x4 rot = Rotation(0.05f, AXIS_Z);
    m_gameObject->SetLocalTransform(trans*rot);
}