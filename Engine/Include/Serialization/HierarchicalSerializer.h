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
    void    AddLeaf(string name, string valueName, T value);
    void    AddLeafVector3(string name, Vector3 value);
    void    AddLeafColorRGB(string name, ColorRGB value);

    void    Save(string filepath);
    void    Load(string filepath);

private:
    XMLDocument         m_document;
    stack<XMLElement*>  m_elementStack;
};