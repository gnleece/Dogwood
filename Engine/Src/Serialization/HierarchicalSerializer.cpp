#include "Serialization/HierarchicalSerializer.h"

void HierarchicalSerializer::PushScope(string name)
{
    XMLElement* newElement = m_document.NewElement(name.c_str());

    if (m_elementStack.size() > 0)
    {
        XMLElement* parentElement = m_elementStack.top();
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
    if (m_elementStack.size() > 0)
    {
        m_elementStack.pop();
    }
}

void HierarchicalSerializer::SetAttributeVector3(Vector3 value)
{
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        element->SetAttribute("x", value.x());
        element->SetAttribute("y", value.y());
        element->SetAttribute("z", value.z());
    }
}

void HierarchicalSerializer::SetAttributeColorRGB(ColorRGB value)
{
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        element->SetAttribute("r", value.r);
        element->SetAttribute("g", value.g);
        element->SetAttribute("b", value.b);
    }
}

void HierarchicalSerializer::InsertLeafVector3(string name, Vector3 value)
{
    PushScope(name);
    SetAttributeVector3(value);
    PopScope();
}

void HierarchicalSerializer::InsertLeafColorRGB(string name, ColorRGB value)
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