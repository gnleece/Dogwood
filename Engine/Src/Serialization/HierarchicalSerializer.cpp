#include "Serialization/HierarchicalSerializer.h"

void HierarchicalSerializer::Save(string filepath)
{
    m_document.SaveFile(filepath.c_str());
}

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

template <>
void HierarchicalSerializer::SetAttribute(string name, string value)
{
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        element->SetAttribute(name.c_str(), value.c_str());
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

//------------------------------------------------------------------------------

HierarchicalDeserializer::HierarchicalDeserializer() : m_loaded(false)
{ }

bool HierarchicalDeserializer::Load(string filepath)
{
    XMLError result = m_document.LoadFile(filepath.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading scene file %s.\nXMLError %d\n", filepath.c_str(), result);
        return false;
    }

    m_loaded = true;
    return true;
}

bool HierarchicalDeserializer::PushScope(string name)
{
    if (!m_loaded)
        return false;

    XMLElement* element = NULL;
    if (m_elementStack.size() > 0)
    {
        XMLElement* parent = m_elementStack.top();
        element = parent->FirstChildElement(name.c_str());
    }
    else
    {
        element = m_document.FirstChildElement(name.c_str());
    }

    if (element != NULL)
    {
        m_elementStack.push(element);
        return true;
    }

    return false;
}

bool HierarchicalDeserializer::PushScope()
{
    if (!m_loaded)
        return false;

    XMLElement* element = NULL;
    if (m_elementStack.size() > 0)
    {
        XMLElement* parent = m_elementStack.top();
        element = parent->FirstChildElement();
    }
    else
    {
        element = m_document.FirstChildElement();
    }

    if (element != NULL)
    {
        m_elementStack.push(element);
        return true;
    }

    return false;
}

void HierarchicalDeserializer::PopScope()
{
    if (m_elementStack.size() > 0)
    {
        m_elementStack.pop();
    }
}

bool HierarchicalDeserializer::NextSiblingScope(string name)
{
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        m_elementStack.pop();

        XMLElement* nextElement = element->NextSiblingElement(name.c_str());
        if (nextElement != NULL)
        {
            m_elementStack.push(nextElement);
            return true;
        }
    }
    return false;
}

bool HierarchicalDeserializer::NextSiblingScope()
{
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        m_elementStack.pop();

        XMLElement* nextElement = element->NextSiblingElement();
        if (nextElement != NULL)
        {
            m_elementStack.push(nextElement);
            return true;
        }
    }
    return false;
}

template <>
bool HierarchicalDeserializer::GetAttribute(string name, int& value)
{
    bool success = false;
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        value = element->IntAttribute(name.c_str());
        success = true;
    }
    return success;
}


template <>
bool HierarchicalDeserializer::GetAttribute(string name, float& value)
{
    bool success = false;
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        value = element->FloatAttribute(name.c_str());
        success = true;
    }
    return success;
}


template <>
bool HierarchicalDeserializer::GetAttribute(string name, bool& value)
{
    bool success = false;
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        value = element->BoolAttribute(name.c_str());
        success = true;
    }
    return success;
}


template <>
bool HierarchicalDeserializer::GetAttribute(string name, string& value)
{
    bool success = false;
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        value = element->Attribute(name.c_str());
        success = true;
    }
    return success;
}

template <>
bool HierarchicalDeserializer::GetAttribute(string name, unsigned int& value)
{
    bool success = false;
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        value = element->UnsignedAttribute(name.c_str());
        success = true;
    }
    return success;
}

bool HierarchicalDeserializer::GetAttributeVector3(Vector3& value)
{
    bool success = false;
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        value.SetX(element->FloatAttribute("x"));
        value.SetY(element->FloatAttribute("y"));
        value.SetZ(element->FloatAttribute("z"));
        success = true;
    }
    return success;
}

bool HierarchicalDeserializer::GetAttributeColorRGB(ColorRGB& value)
{
    bool success = false;
    if (m_elementStack.size() > 0)
    {
        XMLElement* element = m_elementStack.top();
        value.r = element->FloatAttribute("r");
        value.g = element->FloatAttribute("g");
        value.b = element->FloatAttribute("b");
        success = true;
    }
    return success;
}

bool HierarchicalDeserializer::ReadLeafVector3(string name, Vector3& value)
{
    bool success = false;
    if (PushScope(name))
    {
        success = GetAttributeVector3(value);
        PopScope();
    }
    return success;
}

bool HierarchicalDeserializer::ReadLeafColorRGB(string name, ColorRGB& value)
{
    bool success = false;
    if (PushScope(name))
    {
        success = GetAttributeColorRGB(value);
        PopScope();
    }
    return success;
}