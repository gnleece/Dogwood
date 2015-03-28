#include "ToolsideGameComponent.h"
#include "Util.h"

using tinyxml2::XMLElement;

bool ComponentParameter::operator==(const ComponentParameter &other) const
{
    return ((Name.compare(other.Name) == 0) && Type == other.Type);
}

ComponentValue::ComponentValue()
{
    // set default values
    i = 0;
    f = 0.0f;
    b = false;
    str = "";
    v = Vector3::Zero;
    c = ColourRGB::Black;
}

void ToolsideGameComponent::Load(XMLElement* componentXML)
{
    m_guid = componentXML->UnsignedAttribute("guid");

    // Build parameter list
    XMLElement* paramXML = componentXML->FirstChildElement("Param");
    while (paramXML)
    {
        AddParameterToList(paramXML);
        paramXML = paramXML->NextSiblingElement("Param");
    }
}

void ToolsideGameComponent::AddParameterToList(XMLElement* paramXML)
{
    string name = paramXML->Attribute("name");
    // TODO update python script to match enum values instead of using strings
    ComponentParameter::ParameterType type = (ComponentParameter::ParameterType)(paramXML->IntAttribute("type"));
    ComponentParameter key(name, type);

    ComponentValue value;
    char* valueStr = "value";
    if (paramXML->Attribute(valueStr))
    {
        switch (type)
        {
        case ComponentParameter::TYPE_INT:
            value.i = paramXML->IntAttribute(valueStr);
            break;
        case ComponentParameter::TYPE_FLOAT:
            value.f = paramXML->FloatAttribute(valueStr);
            break;
        case ComponentParameter::TYPE_BOOL:
            value.b = paramXML->BoolAttribute(valueStr);
            break;
        case ComponentParameter::TYPE_STRING:
            value.str = paramXML->Attribute(valueStr);
            break;
        case ComponentParameter::TYPE_VECTOR3:
        {
            XMLElement* vectorXML = paramXML->FirstChildElement(valueStr);
            value.v = ReadVector3FromXML(vectorXML);
            break;
        }
        case ComponentParameter::TYPE_COLOR:
        {
            XMLElement* colorXML = paramXML->FirstChildElement(valueStr);
            value.c = ReadColourFromXML(colorXML);
            break;
        }
        }
    }

    m_map[key] = value; 
}