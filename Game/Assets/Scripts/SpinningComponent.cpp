#include "Components\SpinningComponent.h"

#include "Math\Transformations.h"

void SpinningComponent::Update(float deltaTime)
{    
    Matrix4x4 trans = m_gameObject->GetLocalTransform().GetMatrix();
    Matrix4x4 rot = Rotation(Speed*deltaTime, AXIS_Z);
    m_gameObject->SetLocalTransform(trans*rot);
}