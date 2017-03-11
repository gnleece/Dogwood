#include "ToolsideGameComponent.h"
#include "Scene\ResourceManager.h"
#include "Serialization\HierarchicalSerializer.h"
#include "Util.h"
#include <sstream>

bool ComponentParameter::operator==(const ComponentParameter &other) const
{
    return ((Name.compare(other.Name) == 0) && Type == other.Type);
}

bool ComponentParameter::IsReferenceType(ParameterType type)
{
    return (type == TYPE_MESH || type == TYPE_SHADER || type == TYPE_TEXTURE);
}

ComponentValue::ComponentValue()
{
    // set default values
    i = 0;
    f = 0.0f;
    b = false;
    str = "";
    v = Vector3::Zero;
    c = ColorRGB::Black;
    go = 0;
    ref = 0;
}

ComponentValue::ComponentValue(ComponentParameter::ParameterType type, int value)
{
    if (type == ComponentParameter::TYPE_INT)
        i = value;
    else if (type == ComponentParameter::TYPE_FLOAT)
        f = (float)value;
}

ComponentValue::ComponentValue(ComponentParameter::ParameterType type, float value)
{
    if (type == ComponentParameter::TYPE_INT)
        i = (int)value;
    else if (type == ComponentParameter::TYPE_FLOAT)
        f = value;
}

ComponentValue::ComponentValue(ComponentParameter::ParameterType type, bool value)
{
    if (type == ComponentParameter::TYPE_BOOL)
        b = value;
}

ComponentValue::ComponentValue(ComponentParameter::ParameterType type, string value)
{
    if (type == ComponentParameter::TYPE_STRING)
        str = value;
}

ComponentValue::ComponentValue(ComponentParameter::ParameterType type, Vector3 value)
{
    if (type == ComponentParameter::TYPE_VECTOR3)
        v = value;
    else if (type == ComponentParameter::TYPE_COLOR)
        c = value;
}

ComponentValue::ComponentValue(ComponentParameter::ParameterType type, ColorRGB value)
{
    if (type == ComponentParameter::TYPE_COLOR)
        c = value;
}

ComponentValue::ComponentValue(ComponentParameter::ParameterType type, unsigned int value)
{
    if (type == ComponentParameter::TYPE_GAMEOBJECT)
        go = value;
    else if (ComponentParameter::IsReferenceType(type))
        ref = value;
}

ComponentValue::ComponentValue(ComponentParameter::ParameterType type, HierarchicalDeserializer* deserializer)
{
    SetValue(type, deserializer);
}

void ComponentValue::SetValue(ComponentParameter::ParameterType type, HierarchicalDeserializer* deserializer)
{
    char* valueStr = "value";

    switch (type)
    {
        case ComponentParameter::TYPE_INT:
            deserializer->GetAttribute("value", i);
            break;
        case ComponentParameter::TYPE_FLOAT:
            deserializer->GetAttribute("value", f);
            break;
        case ComponentParameter::TYPE_BOOL:
            deserializer->GetAttribute("value", b);
            break;
        case ComponentParameter::TYPE_STRING:
            deserializer->GetAttribute("value", str);
            break;
        case ComponentParameter::TYPE_VECTOR3:
        {
            deserializer->ReadLeafVector3("value", v);
            break;
        }
        case ComponentParameter::TYPE_COLOR:
        {
            deserializer->ReadLeafColorRGB("value", c);
            break;
        }
        case ComponentParameter::TYPE_GAMEOBJECT:
            deserializer->GetAttribute("value", go);
            break;
        case ComponentParameter::TYPE_MESH:
        case ComponentParameter::TYPE_SHADER:
        case ComponentParameter::TYPE_TEXTURE:
            deserializer->GetAttribute("value", ref);
            break;
    }
}

void ComponentValue::SetValue(ComponentParameter::ParameterType type, string text)
{
    switch (type)
    {
        case ComponentParameter::TYPE_INT:
            i = std::stoi(text);
            break;
        case ComponentParameter::TYPE_FLOAT:
            f = std::stof(text);
            break;
        case ComponentParameter::TYPE_BOOL:
            b = (text.compare("true") == 0);    // TODO this field should be a checkbox in the editor      
            break;
        case ComponentParameter::TYPE_STRING:
            str = text;
            break;
        case ComponentParameter::TYPE_VECTOR3:
            v = ReadVector3FromString(text);
            break;
        case ComponentParameter::TYPE_COLOR:
            c = ReadVector3FromString(text);
            break;
        case ComponentParameter::TYPE_GAMEOBJECT:
            go = std::stoul(text);
            break;
        case ComponentParameter::TYPE_MESH:
        case ComponentParameter::TYPE_SHADER:
        case ComponentParameter::TYPE_TEXTURE:
            ref = std::stoul(text);
            break;
    }
}

void ComponentValue::SaveValue(HierarchicalSerializer* serializer, ComponentParameter::ParameterType type)
{
    switch (type)
    {
        case ComponentParameter::TYPE_INT:
            serializer->SetAttribute("value", i);
            break;
        case ComponentParameter::TYPE_FLOAT:
            serializer->SetAttribute("value", f);
            break;
        case ComponentParameter::TYPE_BOOL:
            serializer->SetAttribute("value", b);
            break;
        case ComponentParameter::TYPE_STRING:
            serializer->SetAttribute("value", str.c_str());
            break;
        case ComponentParameter::TYPE_VECTOR3:
            serializer->SetAttribute("value", "");
            serializer->InsertLeafVector3("value", v);
            break;
        case ComponentParameter::TYPE_COLOR:
            serializer->SetAttribute("value", "");
            serializer->InsertLeafColorRGB("value", c);
            break;
        case ComponentParameter::TYPE_GAMEOBJECT:
            serializer->SetAttribute("value", go);
            break;
        case ComponentParameter::TYPE_MESH:
        case ComponentParameter::TYPE_SHADER:
        case ComponentParameter::TYPE_TEXTURE:
            serializer->SetAttribute("value", ref);
            break;
    }
}

string ComponentValue::GetValueString(ComponentParameter::ParameterType type)
{
    switch (type)
    {
        case ComponentParameter::TYPE_INT:          return std::to_string(i);
        case ComponentParameter::TYPE_FLOAT:        return std::to_string(f);
        case ComponentParameter::TYPE_BOOL:         return b ? "true" : "false";
        case ComponentParameter::TYPE_STRING:       return str;
        case ComponentParameter::TYPE_VECTOR3:      return WriteVector3ToString(v);
        case ComponentParameter::TYPE_COLOR:        return WriteVector3ToString(c.ToVector());
        case ComponentParameter::TYPE_GAMEOBJECT:   return std::to_string(go);
        case ComponentParameter::TYPE_MESH:
        case ComponentParameter::TYPE_SHADER:
        case ComponentParameter::TYPE_TEXTURE:      return std::to_string(ref);
    }

    return "";
}

RuntimeParamList ComponentValue::ParseRuntimeParams(HierarchicalDeserializer* deserializer)
{
    RuntimeParamList params;

    bool paramsToProcess = deserializer->PushScope("Param");
    while (paramsToProcess)
    {
        ComponentParameter::ParameterType type;
        deserializer->GetAttribute("type", (int&)type);

        ComponentValue value(type, deserializer);
        params.push_back(value);

        paramsToProcess = deserializer->NextSiblingScope("Param");
    }

    return params;
}

void ToolsideGameComponent::Create(unsigned int guid, bool isEngine)
{
    m_resourceGuid = guid;
    m_isEngine = isEngine;
    m_paramList = *(ResourceManager::Singleton().GetComponentParamList(m_resourceGuid, m_isEngine));
    SetDisplayName();
}

void ToolsideGameComponent::Save(HierarchicalSerializer* serializer, unordered_set<unsigned int>& guids)
{
    serializer->PushScope("Component");
    serializer->SetAttribute("guid", m_resourceGuid);
    serializer->SetAttribute("engine", m_isEngine);

    ParamList::iterator iter = m_paramList.begin();
    for (; iter != m_paramList.end(); iter++)
    {
        ParamPair pair = *iter;

        serializer->PushScope("Param");

        serializer->SetAttribute("name", pair.first.Name.c_str());
        serializer->SetAttribute("type", pair.first.Type);
        pair.second.SaveValue(serializer, pair.first.Type);

        if (ComponentParameter::IsReferenceType(pair.first.Type))
        {
            guids.insert(pair.second.ref);
        }

        serializer->PopScope();
    }

    serializer->PopScope();
}

void ToolsideGameComponent::Load(HierarchicalDeserializer* deserializer)
{
    deserializer->GetAttribute("guid", m_resourceGuid);
    deserializer->GetAttribute("engine", m_isEngine);

    // Build parameter list
    bool paramsToProcess = deserializer->PushScope("Param");
    while (paramsToProcess)
    {
        AddParameterToList(deserializer);
        paramsToProcess = deserializer->NextSiblingScope("Param");
    }
    SetDisplayName();
    ValidateParameters();
}

ToolsideGameObject* ToolsideGameComponent::GetGameObject()
{
    return m_gameObject;
}

void ToolsideGameComponent::SetGameObject(ToolsideGameObject* go)
{
    m_gameObject = go;
}

unsigned int ToolsideGameComponent::GetGuid()
{
    return m_resourceGuid;
}

string ToolsideGameComponent::GetDisplayName()
{
    return m_displayName;
}

ParamList& ToolsideGameComponent::GetParameterList()
{
    return m_paramList;
}

void ToolsideGameComponent::SetParameter(ComponentParameter param, ComponentValue value)
{
    ParamList::iterator iter = m_paramList.begin();
    for (; iter != m_paramList.end(); iter++)
    {
        ComponentParameter existingParam = iter->first;
        if (existingParam == param)
        {
            iter->second = value;
            break;
        }
    }
}

void ToolsideGameComponent::ValidateParameters()
{
    // Validate parameter list by removing any parameters that no longer exist in the schema, and adding
    // any new values that did not exist before (while preserving non-default values for params that remain)

    ParamList currentParams = *(ResourceManager::Singleton().GetComponentParamList(m_resourceGuid, m_isEngine));

    // This is pretty inefficient, but the list of parameters will typically be small
    ParamList::iterator oldIter = m_paramList.begin();
    for (; oldIter != m_paramList.end(); oldIter++)
    {
        // If this entry from the old list still exists in the new list, copy over its value
        ComponentParameter oldParam = oldIter->first;
        ParamList::iterator newIter = currentParams.begin();
        for (; newIter != currentParams.end(); newIter++)
        {
            ComponentParameter newParam = newIter->first;
            if (newParam == oldParam)
            {
                newIter->second = oldIter->second;
                continue;
            }
        }
    }
    m_paramList = currentParams;
}

ToolsideGameComponent* ToolsideGameComponent::DeepCopy(ToolsideGameObject* go)
{
    ToolsideGameComponent* newComp = new ToolsideGameComponent();
    newComp->SetGameObject(go);

    newComp->m_resourceGuid = m_resourceGuid;
    newComp->m_isEngine = m_isEngine;
    newComp->m_displayName = m_displayName;

    ParamList::iterator iter = m_paramList.begin();
    for (; iter != m_paramList.end(); iter++)
    {
        ParamPair pair = *iter;
        newComp->m_paramList.push_back(pair);
    }

    return newComp;
}

void ToolsideGameComponent::AddParameterToList(HierarchicalDeserializer* deserializer)
{
    string name;
    deserializer->GetAttribute("name", name);

    ComponentParameter::ParameterType type;
    deserializer->GetAttribute("type", (int&)type);
    ComponentParameter key(name, type);

    ComponentValue value;
    value.SetValue(type, deserializer);

    ParamPair pair(key, value);
    m_paramList.push_back(pair);
}

void ToolsideGameComponent::SetDisplayName()
{
    ResourceInfo* info = ResourceManager::Singleton().GetResourceInfo(m_resourceGuid);
    if (info == NULL)
    {
        // TODO this happens for Engine components, needs a workaround
        return;
    }

    string path = info->path;
    unsigned int startpos = path.find_last_of('/') + 1;
    unsigned int endpos = path.find_last_of('.') - 1;
    unsigned int length = endpos - startpos + 1;
    m_displayName = path.substr(startpos, length);
}

bool ToolsideComponentSchema::Load(string filename)
{
    Unload();

    // Open the schema doc
    HierarchicalDeserializer deserializer;
    bool success = deserializer.Load(filename);
    if (!success)
    {
        printf("Error reading component schema file %s.\n", filename.c_str());
        return false;
    }

    // Process each script in the schema
    deserializer.PushScope("Scripts");
    bool scriptsToProcess = deserializer.PushScope("Script");
    while (scriptsToProcess)
    {
        // Build the param map for this script
        ParamList* paramList = new ParamList();
        bool paramsToProcess = deserializer.PushScope("Param");
        while (paramsToProcess)
        {
            ComponentParameter param;
            deserializer.GetAttribute("name", param.Name);
            deserializer.GetAttribute("type", (int&)param.Type);

            ComponentValue value;
            value.SetValue(param.Type, &deserializer);

            ParamPair pair(param, value);
            paramList->push_back(pair);

            paramsToProcess = deserializer.NextSiblingScope("Param");
        }

        // Add the map to the schema
        unsigned int guid;
        deserializer.GetAttribute("guid", guid);
        m_schema[guid] = paramList;

        // Move to the next script
        scriptsToProcess = deserializer.NextSiblingScope("Script");
    }
    deserializer.PopScope();

    return true;
}

void ToolsideComponentSchema::Unload()
{
    unordered_map<unsigned int, ParamList*>::iterator iter;
    for (iter = m_schema.begin(); iter != m_schema.end(); iter++)
    {
        delete m_schema[iter->first];
    }
    m_schema.clear();
}

ParamList* ToolsideComponentSchema::GetDefaultParameterList(unsigned int guid)
{
    return m_schema[guid];
}