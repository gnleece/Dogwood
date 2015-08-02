#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

typedef vector <string> ShaderParamList;

class ToolsideShaderSchema
{
public:
    bool                Load(string filename);
    void                Unload();
    ShaderParamList*    GetParameterList(unsigned int guid);

private:
    unordered_map<unsigned int, ShaderParamList*> m_schema;
};