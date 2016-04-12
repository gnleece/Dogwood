#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Math\Algebra.h"
#include "Rendering\Color.h"

using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct ComponentParameter;
struct ComponentValue;

class HierarchicalDeserializer;
class HierarchicalSerializer;
class ToolsideGameObject;

typedef pair <ComponentParameter, ComponentValue> ParamPair;
typedef vector <ParamPair> ParamList;
typedef vector<ComponentValue> RuntimeParamList;

struct ComponentParameter
{
public:
    enum ParameterType
    {
        TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING, TYPE_VECTOR3, TYPE_COLOR, TYPE_GAMEOBJECT,
        TYPE_MESH, TYPE_SHADER, TYPE_TEXTURE
    };

    ComponentParameter() {}
    ComponentParameter(string name, ParameterType type)
    {
        Name = name; Type = type;
    }

    bool operator==(const ComponentParameter &other) const;
    static bool IsReferenceType(ParameterType type);

    string          Name;
    ParameterType   Type;
};

// TODO this should really be a Union not a Struct, but I'm having trouble getting the Union to work
struct ComponentValue
{
public:
    ComponentValue();
    ComponentValue(ComponentParameter::ParameterType type, int value);
    ComponentValue(ComponentParameter::ParameterType type, float value);
    ComponentValue(ComponentParameter::ParameterType type, bool value);
    ComponentValue(ComponentParameter::ParameterType type, string value);
    ComponentValue(ComponentParameter::ParameterType type, Vector3 value);
    ComponentValue(ComponentParameter::ParameterType type, ColorRGB value);
    ComponentValue(ComponentParameter::ParameterType type, unsigned int value);
    ComponentValue(ComponentParameter::ParameterType type, HierarchicalDeserializer* deserializer);

    void    SetValue(ComponentParameter::ParameterType type, HierarchicalDeserializer* deserializer);
    void    SetValue(ComponentParameter::ParameterType type, string text);
    void    SaveValue(HierarchicalSerializer* serializer, ComponentParameter::ParameterType type);
    string  GetValueString(ComponentParameter::ParameterType type);
    string  GetVectorValueString(Vector3 v);

    static RuntimeParamList ParseRuntimeParams(HierarchicalDeserializer* deserializer);

    int             i;
    float           f;
    bool            b;
    string          str;
    Vector3         v;
    ColorRGB        c;
    unsigned int    go;
    unsigned int    ref;
};

class ToolsideGameComponent
{
public:
    void            Create(unsigned int guid, bool isEngine);
    void            Save(HierarchicalSerializer* serializer, unordered_set<unsigned int>& guids);
    void            Load(HierarchicalDeserializer* deserializer);

    ToolsideGameObject* GetGameObject();
    void            SetGameObject(ToolsideGameObject* go);

    unsigned int    GetGuid();
    string          GetDisplayName();

    ParamList&      GetParameterList();
    void            SetParameter(ComponentParameter param, ComponentValue value);

    void            ValidateParameters();

private:
    void            AddParameterToList(HierarchicalDeserializer* deserializer);
    void            SetDisplayName();

    ToolsideGameObject* m_gameObject;
    unsigned int    m_guid;
    bool            m_isEngine;
    string          m_displayName;
    ParamList       m_paramList;
};

class ToolsideComponentSchema
{
public:
    bool            Load(string filename);
    void            Unload();
    ParamList*      GetDefaultParameterList(unsigned int guid);

private:
    unordered_map<unsigned int, ParamList*> m_schema;
};