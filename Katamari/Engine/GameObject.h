#pragma once

#include <list>
#include <string>

#include "Math\Algebra.h"
#include "GameComponent.h"

using std::string;
using std::list;

class GameObject
{
public:    
    const Matrix4x4& GetTransform()         { return m_transform; }
    void SetTransform(const Matrix4x4& m)   { m_transform = m; }

    int GetID()                             { return m_id; }
    string GetName()                        { return m_name; }

    void Start();
    void Update(float deltaTime);
    void Render();

private:
    int     m_id;
    string  m_name;

    Matrix4x4   m_transform;

    GameObject*             m_parent;
    list<GameObject*>       m_children;

    list<GameComponent*>    m_components;
};