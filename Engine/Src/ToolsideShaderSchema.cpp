#include "ToolsideShaderSchema.h"
#include "tinyxml2.h"

using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLError;

bool ToolsideShaderSchema::Load(string filename)
{
    Unload();

    // Open the schema doc
    XMLDocument schemaDoc;
    XMLError result = schemaDoc.LoadFile(filename.c_str());
    if (result != tinyxml2::XML_SUCCESS)
    {
        printf("Error reading shader schema file %s.\nXMLError %d\n", filename.c_str(), result);
        return false;
    }

    // Process each script in the schema
    XMLElement* scriptsXML = schemaDoc.FirstChildElement("Shaders");
    XMLElement* scriptXML = scriptsXML->FirstChildElement("Shader");
    while (scriptXML != NULL)
    {
        // Build the param map for this script
        ShaderParamList* paramList = new ShaderParamList();
        XMLElement* paramXML = scriptXML->FirstChildElement("Param");
        while (paramXML != NULL)
        {
            ShaderParamType type = (ShaderParamType)(paramXML->IntAttribute("type"));
            string name = paramXML->Attribute("name");

            ShaderParamPair paramPair(type, name);
            paramList->push_back(paramPair);
            paramXML = paramXML->NextSiblingElement("Param");
        }

        // Add the map to the schema
        unsigned int guid = scriptXML->UnsignedAttribute("guid");
        m_schema[guid] = paramList;

        // Move to the next script
        scriptXML = scriptXML->NextSiblingElement("Shader");
    }

    return true;
}

void ToolsideShaderSchema::Unload()
{
    unordered_map<unsigned int, ShaderParamList*>::iterator iter;
    for (iter = m_schema.begin(); iter != m_schema.end(); iter++)
    {
        delete m_schema[iter->first];
    }
    m_schema.clear();
}

ShaderParamList* ToolsideShaderSchema::GetParameterList(unsigned int guid)
{
    return m_schema[guid];
}