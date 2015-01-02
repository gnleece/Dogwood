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

struct ResourceInfo
{
    virtual void        AddToMap(tinyxml2::XMLElement* element, unordered_map<int, ResourceInfo*> & map);
    virtual void        Serialize(tinyxml2::XMLDocument& rootDoc, tinyxml2::XMLElement* parent);

    virtual Resource*   Load() = 0;
    void                Unload();
    virtual string      TypeName() = 0;

    int     guid;
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

    void            LoadResourceMap(tinyxml2::XMLElement* resources);
    void            SerializeResourceMap(tinyxml2::XMLDocument& rootDoc, tinyxml2::XMLElement* parent);
    bool            ImportResource(string filepath, string type);

    void            LoadSceneResources(tinyxml2::XMLElement* resources);
    void            UnloadSceneResources();

    Resource*       GetResource(int guid);
    Texture*        GetTexture(int guid);
    Mesh*           GetMesh(int guid);
    ShaderProgram*  GetShader(int guid);

private:
    void ClearResourceLookupTable();

    template<typename T>
    void AddResourcesToMap(tinyxml2::XMLElement* resources, string typeName);

    unordered_map<int, ResourceInfo*> m_resourceLookup;
    unordered_map<int, Resource*> m_loadedResources;

    bool m_lookupTableLoaded;
};