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

    void    SetValue(ComponentParameter::ParameterType type, tinyxml2::XMLElement* xml);
    string  GetValueString(ComponentParameter::ParameterType type);

    int         i;
    float       f;
    bool        b;
    string      str;
    Vector3     v;
    ColourRGB   c;
};

namespace std
{
    template <> struct hash<ComponentParameter>
    {
        size_t operator()(const ComponentParameter & c) const
        {
            using std::size_t;
            using std::hash;

            // TODO this hash function is terrible, fix it
            return ((hash<string>()(c.Name)
                ^ (hash<int>()((int)c.Type) << 1)) >> 1);
        }
    };
}

typedef pair <ComponentParameter, ComponentValue> ParamPair;
typedef vector <ParamPair> ParamList;

class ToolsideGameComponent
{
public:
    void            Create(unsigned int guid);
    void            Load(tinyxml2::XMLElement* componentXML);
    void            Serialize();

    ParamList&      GetParameterList();
    void            SetParameter(ComponentParameter param, ComponentValue value);

    unsigned int    GetGuid();

private:
    void            AddParameterToList(tinyxml2::XMLElement* paramXML);

    unsigned int    m_guid;
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