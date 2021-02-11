#include "Translator.h"

#include "GameObject.h"
#include "Math\Transformations.h"

void Translator::Update(float deltaTime)
{
    Matrix4x4 m = m_gameObject->GetTransform().GetLocalMatrix();
    Matrix4x4 t = Translation(Vector3(0.0f, deltaTime * Speed, 0.0f));
    m_gameObject->GetTransform().SetLocalMatrix(m*t);
}