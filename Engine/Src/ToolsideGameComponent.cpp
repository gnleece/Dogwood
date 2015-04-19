#include "ToolsideGameComponent.h"
#include "Util.h"

using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLError;

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

void ComponentValue::SetValue(ComponentParameter::ParameterType type, tinyxml2::XMLElement* xml)
{
    char* valueStr = "value";
    if (xml->Attribute(valueStr))
    {
        switch (type)
        {
        case ComponentParameter::TYPE_INT:
            i = xml->IntAttribute(valueStr);
            break;
        case ComponentParameter::TYPE_FLOAT:
            f = xml->FloatAttribute(valueStr);
            break;
        case ComponentParameter::TYPE_BOOL:
            b = xml->BoolAttribute(valueStr);
            break;
        case ComponentParameter::TYPE_STRING:
            str = xml->Attribute(valueStr);
            break;
        case ComponentParameter::TYPE_VECTOR3:
        {
            XMLElement* vectorXML = xml->FirstChildElement(valueStr);
            v = ReadVector3FromXML(vectorXML);
            break;
        }
        case ComponentParameter::TYPE_COLOR:
        {
            XMLElement* colorXML = xml->FirstChildElement(valueStr);
            c = ReadColourFromXML(colorXML);
            break;
        }
        }
    }
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

    ComponentParameter::ParameterType type = (ComponentParameter::ParameterType)(paramXML->IntAttribute("type"));
    ComponentParameter key(name, type);

    ComponentValue value;
    value.SetValue(type, paramXML);

    m_map[key] = value;
}

bool ToolsideComponentSchema::Load(string filename)
{
    Unload();

    // Open the schema doc
    XMLDocument schemaDoc;
    XMLError result = schemaDoc.LoadFile(filename.c_str());
    if (result != tinyxml2::XML_SUCCESS)
    {
        printf("Error reading schema file %s.\nXMLError %d\n", filename.c_str(), result);
        return false;
    }

    // Process each script in the schema
    XMLElement* scriptsXML = schemaDoc.FirstChildElement("Scripts");
    XMLElement* scriptXML = scriptsXML->FirstChildElement("Script");
    while (scriptXML != NULL)
    {
        // Build the param map for this script
        ParamMap* map = new ParamMap();
        XMLElement* paramXML = scriptXML->FirstChildElement("Param");
        while (paramXML != NULL)
        {
            ComponentParameter param;
            param.Name = paramXML->Attribute("name");
            param.Type = (ComponentParameter::ParameterType)(paramXML->IntAttribute("type"));

            ComponentValue value;
            value.SetValue(param.Type, paramXML);

            (*map)[param] = value;
            paramXML = paramXML->NextSiblingElement("Param");
        }

        // Add the map to the schema
        unsigned int guid = scriptXML->UnsignedAttribute("guid");
        m_schema[guid] = map;

        // Move to the next script
        scriptXML = scriptXML->NextSiblingElement("Script");
    }

    return true;
}

void ToolsideComponentSchema::Unload()
{
    unordered_map<unsigned int, ParamMap*>::iterator iter;
    for (iter = m_schema.begin(); iter != m_schema.end(); iter++)
    {
        delete m_schema[iter->first];
    }
    m_schema.clear();
}

ParamMap* ToolsideComponentSchema::GetDefaultParameterList(unsigned int guid)
{
    return m_schema[guid];
}