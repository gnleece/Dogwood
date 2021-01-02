#include "pch.h"

#include "SpinningComponent.h"

#include "GameObject.h"
#include "Math\Transformations.h"

void SpinningComponent::Update(float deltaTime)
{    
    Matrix4x4 trans = m_gameObject->GetTransform().GetLocalMatrix();
    Matrix4x4 rot = Rotation(Speed*deltaTime, AXIS_Z);
    m_gameObject->GetTransform().SetLocalMatrix(trans*rot);
}