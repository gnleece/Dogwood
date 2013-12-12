#pragma once

#include <list>
#include <string>

#include "Math\Algebra.h"
#include "Primitive.h"

using std::string;

class SceneNode;
typedef std::list<SceneNode*> NodeList;

class SceneNode
{
public:
    virtual void Render();
    
    const Matrix4x4& GetTransform() const { return m_transform; }
    void SetTransform(const Matrix4x4& m) { m_transform = m; }

    void SetID(int id) { m_id = id; }
    int GetID() { return m_id; }

protected:
    int         m_id;
    string      m_name;

    Matrix4x4   m_transform;

    SceneNode*  m_parent;
    NodeList    m_children;
};

class GeometryNode : public SceneNode
{
public:

protected:
    Primitive*    m_primitive;
    //Material*   m_material;
};