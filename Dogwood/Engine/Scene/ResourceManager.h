#pragma once

#include <string>
#include <tinyxml2.h>
#include <unordered_map>

#include "Resource.h"

class Mesh;
class ShaderProgram;
class Texture;

using std::string;
using std::unordered_map;
using namespace tinyxml2;

struct ResourceInfo
{
    virtual void        AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map);
    virtual Resource*   Load() = 0;
    void                Unload();

    string path;
};

class ResourceManager
{
public:
    static ResourceManager& Singleton()
    {
        static ResourceManager    singleton;
        return singleton;
    }
    ResourceManager() {}

    void            Startup();
    void            Shutdown();

    void            LoadSceneResources(string filepath);    // TODO file should be loaded elsewhere and this should be given an XMLElement*
    void            UnloadSceneResources();

    Resource*       GetResource(int guid);
    Texture*        GetTexture(int guid);
    Mesh*           GetMesh(int guid);
    ShaderProgram*  GetShader(int guid);

private:
    void BuildResourceLookupTable();
    void ClearResourceLookupTable();

    void LoadResourcesOfType(XMLElement* resources, string typeName);

    unordered_map<int, ResourceInfo*> m_resourceLookup;
    unordered_map<int, Resource*> m_loadedResources;
};