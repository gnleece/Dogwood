#include "TestComponent.h"

#include "..\Engine\Math\Transformations.h"

void TestComponent::Update(float deltaTime)
{    
    Matrix4x4 trans = m_gameObject->GetLocalTransform().GetMatrix();
    Matrix4x4 rot = Rotation(50*deltaTime, AXIS_Z);
    m_gameObject->SetLocalTransform(trans*rot);
}