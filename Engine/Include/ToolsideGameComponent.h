#pragma once

#include <string>
#include <tinyxml2.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Math\Algebra.h"
#include "Rendering\Colour.h"

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

struct ComponentParameter;
struct ComponentValue;

typedef pair <ComponentParameter, ComponentValue> ParamPair;
typedef vector <ParamPair> ParamList;
typedef vector<ComponentValue> RuntimeParamList;

struct ComponentParameter
{
public:
    enum ParameterType
    {
        TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING, TYPE_VECTOR3, TYPE_COLOR
    };

    ComponentParameter() {}
    ComponentParameter(string name, ParameterType type)
    {
        Name = name; Type = type;
    }

    bool operator==(const ComponentParameter &other) const;

    string          Name;
    ParameterType   Type;
};

// TODO this should really be a Union not a Struct, but I'm having trouble getting the Union to work
struct ComponentValue
{
public:
    ComponentValue();
    ComponentValue(ComponentParameter::ParameterType type, tinyxml2::XMLElement* xml);

    void    SetValue(ComponentParameter::ParameterType type, tinyxml2::XMLElement* xml);
    void    SetValue(ComponentParameter::ParameterType type, string text);
    void    SerializeValue(ComponentParameter::ParameterType type, tinyxml2::XMLElement* parentNode, tinyxml2::XMLDocument& rootDoc);
    string  GetValueString(ComponentParameter::ParameterType type);

    static RuntimeParamList ParseRuntimeParams(tinyxml2::XMLElement* xml);

    int         i;
    float       f;
    bool        b;
    string      str;
    Vector3     v;
    ColourRGB   c;
};

class ToolsideGameComponent
{
public:
    void            Create(unsigned int guid);
    void            Load(tinyxml2::XMLElement* componentXML);
    void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);

    unsigned int    GetGuid();
    string          GetDisplayName();

    ParamList&      GetParameterList();
    void            SetParameter(ComponentParameter param, ComponentValue value);

    void            ValidateParameters();

private:
    void            AddParameterToList(tinyxml2::XMLElement* paramXML);
    void            SetDisplayName();

    unsigned int    m_guid;
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