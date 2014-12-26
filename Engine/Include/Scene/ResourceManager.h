#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define QT_NO_OPENGL_ES_2

#include <string>
#include <tinyxml2.h>
#include <unordered_map>

#include "Resource.h"

class GameProject;
class Mesh;
class ShaderProgram;
class Texture;

using std::string;
using std::unordered_map;
using namespace tinyxml2;

struct ResourceInfo
{
    virtual void        AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map);
    virtual void        Serialize(XMLDocument& rootDoc, XMLElement* parent);

    virtual Resource*   Load() = 0;
    void                Unload();

    int     guid;
    string  typeName;
    string  path;
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

    void            LoadResourceMap(XMLElement* resources);
    void            SerializeResourceMap(XMLDocument& rootDoc, XMLElement* parent);
    bool            AddResourceToMap(ResourceInfo* resource, int guid);  // TODO this function should create its own guids

    void            LoadSceneResources(XMLElement* resources);
    void            UnloadSceneResources();

    Resource*       GetResource(int guid);
    Texture*        GetTexture(int guid);
    Mesh*           GetMesh(int guid);
    ShaderProgram*  GetShader(int guid);

private:
    void ClearResourceLookupTable();

    template<typename T>
    void AddResourcesToMap(XMLElement* resources, string typeName);

    unordered_map<int, ResourceInfo*> m_resourceLookup;
    unordered_map<int, Resource*> m_loadedResources;
};