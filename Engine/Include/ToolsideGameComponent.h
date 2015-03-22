#pragma once

#include <string>
#include <unordered_map>

#include "Math\Algebra.h"
#include "Rendering\Colour.h"

using std::string;
using std::unordered_map;

struct ComponentParameter
{
public:
    enum ParameterType
    {
        TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING, TYPE_VECTOR3, TYPE_COLOR
    };

    string          Name;
    ParameterType   Type;
};

// TODO this should really be a Union not a Struct, but I'm having trouble getting the Union to work
struct ComponentValue
{
public:
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

typedef unordered_map < ComponentParameter, ComponentValue > ParamMap;

class ToolsideGameComponent
{
public:
    void            Create(int guid);
    void            Load();
    void            Serialize();

    ParamMap*       GetParameterList();
    void            SetParameter(ComponentParameter param, ComponentValue value);

private:
    unsigned int    m_guid;
    ParamMap        m_map;
};