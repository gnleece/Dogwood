#pragma once

#include <stack>
#include <string>
#include "tinyxml2.h"

#include "Math/Algebra.h"
#include "Rendering/Color.h"

using std::stack;
using std::string;
using namespace tinyxml2;

class HierarchicalSerializer
{
public:
    void    PushScope(string name);
    void    PopScope();

    template<typename T>
    void    SetAttribute(string name, T value);
    void    SetAttributeVector3(Vector3 value);
    void    SetAttributeColorRGB(ColorRGB value);

    template<typename T>
    void    InsertLeaf(string name, string valueName, T value);
    void    InsertLeafVector3(string name, Vector3 value);
    void    InsertLeafColorRGB(string name, ColorRGB value);

    void    Save(string filepath);
    void    Load(string filepath);

private:
    XMLDocument         m_document;
    stack<XMLElement*>  m_elementStack;
};

template<typename T>
void HierarchicalSerializer::SetAttribute(string name, T value)
{
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        element->SetAttribute(name.c_str(), value);
    }
}

template<typename T>
void HierarchicalSerializer::InsertLeaf(string name, string valueName, T value)
{
    PushScope(name);
    SetAttribute(valueName, value);
    PopScope();
}