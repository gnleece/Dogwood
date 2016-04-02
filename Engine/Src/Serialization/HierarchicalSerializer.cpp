#include "Serialization/HierarchicalSerializer.h"

void HierarchicalSerializer::PushScope(string name)
{
    XMLElement* newElement = m_document.NewElement(name.c_str());
    XMLElement* parentElement = m_elementStack.top();
    if (parentElement != NULL)
    {
        parentElement->InsertEndChild(newElement);
    }
    else
    {
        m_document.InsertEndChild(newElement);
    }
    m_elementStack.push(newElement);
}

void HierarchicalSerializer::PopScope()
{
    m_elementStack.pop();
}

template<typename T>
void HierarchicalSerializer::SetAttribute(string name, T value)
{
    XMLElement* element = m_elementStack.top();
    if (element != NULL)
    {
        element->SetAttribute(name.c_str(), value);
    }
}

void HierarchicalSerializer::SetAttributeVector3(Vector3 value)
{
    XMLElement* element = m_elementStack.top();
    if (element != NULL)
    {
        element->SetAttribute("x", value.x());
        element->SetAttribute("y", value.y());
        element->SetAttribute("z", value.z());
    }
}

void HierarchicalSerializer::SetAttributeColorRGB(ColorRGB value)
{
    XMLElement* element = m_elementStack.top();
    if (element != NULL)
    {
        element->SetAttribute("r", value.r);
        element->SetAttribute("g", value.g);
        element->SetAttribute("b", value.b);
    }
}

template<typename T>
void HierarchicalSerializer::AddLeaf(string name, string valueName, T value)
{
    PushScope(name);
    SetAttribute(valueName, value);
    PopScope();
}

void HierarchicalSerializer::AddLeafVector3(string name, Vector3 value)
{
    PushScope(name);
    SetAttributeVector3(value);
    PopScope();
}

void HierarchicalSerializer::AddLeafColorRGB(string name, ColorRGB value)
{
    PushScope(name);
    SetAttributeColorRGB(value);
    PopScope();
}

void HierarchicalSerializer::Save(string filepath)
{
    m_document.SaveFile(filepath.c_str());
}

void HierarchicalSerializer::Load(string filepath)
{
    // TODO implement me
}