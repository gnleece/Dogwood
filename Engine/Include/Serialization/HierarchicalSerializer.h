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
    void    Save(string filepath);

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

private:
    XMLDocument         m_document;
    stack<XMLElement*>  m_elementStack;
};

class HierarchicalDeserializer
{
public:
    HierarchicalDeserializer();

    bool    Load(string filepath);

    bool    PushScope(string name);
    bool    PushScope();
    void    PopScope();
    bool    NextSiblingScope(string name);
    bool    NextSiblingScope();

    template<typename T>
    bool    GetAttribute(string name, T& value);
    bool    GetAttributeVector3(Vector3& value);
    bool    GetAttributeColorRGB(ColorRGB& value);

    template<typename T>
    bool    ReadLeaf(string name, string valueName, T& value);
    bool    ReadLeafVector3(string name, Vector3& value);
    bool    ReadLeafColorRGB(string name, ColorRGB& value);

private:
    XMLDocument         m_document;
    stack<XMLElement*>  m_elementStack;

    bool                m_loaded;
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

template <> void HierarchicalSerializer::SetAttribute(string name, string value);

template<typename T>
void HierarchicalSerializer::InsertLeaf(string name, string valueName, T value)
{
    PushScope(name);
    SetAttribute(valueName, value);
    PopScope();
}

template<typename T>
bool HierarchicalDeserializer::ReadLeaf(string name, string valueName, T& value)
{
    bool success = false;
    if (PushScope(name))
    {
        success = GetAttribute(valueName, &value);
        PopScope();
    }
    return success;
}

template <> bool HierarchicalDeserializer::GetAttribute(string name, int& value);
template <> bool HierarchicalDeserializer::GetAttribute(string name, float& value);
template <> bool HierarchicalDeserializer::GetAttribute(string name, bool& value);
template <> bool HierarchicalDeserializer::GetAttribute(string name, string& value);
template <> bool HierarchicalDeserializer::GetAttribute(string name, unsigned int& value);