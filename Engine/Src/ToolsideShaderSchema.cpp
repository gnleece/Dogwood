#include "ToolsideShaderSchema.h"
#include "Serialization/HierarchicalSerializer.h"

bool ToolsideShaderSchema::Load(string filename)
{
    Unload();

    // Open the schema doc
    HierarchicalDeserializer deserializer;
    bool success = deserializer.Load(filename);
    if (!success)
    {
        printf("Error reading shader schema file %s.\n", filename.c_str());
        return false;
    }

    // Process each script in the schema
    deserializer.PushScope("Shaders");
    bool scriptsToProcess = deserializer.PushScope("Shader");
    while (scriptsToProcess)
    {
        // Build the param map for this script
        ShaderParamList* paramList = new ShaderParamList();
        bool paramsToProcess = deserializer.PushScope("Param");
        while (paramsToProcess)
        {
            int type;
            deserializer.GetAttribute("type", type);

            string name;
            deserializer.GetAttribute("name", name);

            ShaderParamPair paramPair((ShaderParamType)type, name);
            paramList->push_back(paramPair);

            paramsToProcess = deserializer.NextSiblingScope("Param");
        }

        // Add the map to the schema
        unsigned int guid;
        deserializer.GetAttribute("guid", guid);
        m_schema[guid] = paramList;

        // Move to the next script
        scriptsToProcess = deserializer.NextSiblingScope("Shader");
    }
    deserializer.PopScope();

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