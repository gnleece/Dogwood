#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

enum ShaderParamType { SHADERPARAM_COLOR, SHADERPARAM_FLOAT, SHADERPARAM_TEXTURE };

typedef pair <ShaderParamType, string> ShaderParamPair;
typedef vector <ShaderParamPair> ShaderParamList;

class ToolsideShaderSchema
{
public:
    bool                Load(string filename);
    void                Unload();
    ShaderParamList*    GetParameterList(unsigned int guid);

private:
    unordered_map<unsigned int, ShaderParamList*> m_schema;
};