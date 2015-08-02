#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define QT_NO_OPENGL_ES_2

#include <string>
#include <tinyxml2.h>
#include <unordered_map>

#include "Resource.h"
#include "ToolsideGameComponent.h"
#include "ToolsideShaderSchema.h"

class GameObject;
class GameProject;
class Mesh;
class ShaderProgram;
class Texture;

using std::string;
using std::unordered_map;

class ResourceInfo
{
public:
    virtual void        AddToMap(tinyxml2::XMLElement* element, unordered_map<unsigned int, ResourceInfo*> & map);
    virtual void        Serialize(tinyxml2::XMLDocument& rootDoc, tinyxml2::XMLElement* parent);

    virtual Resource*   Load() = 0;
    virtual void        Unload();
    virtual string      TypeName() = 0;
    virtual void        AddToGameObject(GameObject* gameObject) = 0;

    unsigned int        guid;
    string              path;
};

typedef unordered_map<unsigned int, ResourceInfo*> ResourceMap;

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
    void            ClearResourceMap();
    void            SerializeResourceMap(tinyxml2::XMLDocument& rootDoc, tinyxml2::XMLElement* parent);
    void            LoadComponentSchema();
    void            LoadShaderSchema();

    unsigned int    ImportResource(string& filepath, string type);
    void            ImportDefaultResources();

    void            LoadSceneResources(tinyxml2::XMLElement* resources);
    void            UnloadSceneResources();

    Resource*       GetResource(unsigned int guid, bool load = true);
    Texture*        GetTexture(unsigned int guid, bool load = true);
    Mesh*           GetMesh(unsigned int guid, bool load = true);
    ShaderProgram*  GetShader(unsigned int guid, bool load = true);
    Resource*       GetDefaultResource(string name, bool load = true);

    ResourceInfo*   GetResourceInfo(unsigned int guid);

    void            SetResourceBasePath(string& path);
    string          GetResourceBasePath();

    string          AbsolutePathToProjectPath(string& absolutePath);

    ResourceMap&    GetResourceMap();
    ParamList*      GetComponentParamList(unsigned int guid);
    ShaderParamList* GetShaderParamList(unsigned int guid);

private:
    unsigned int    Import(ResourceInfo* resource);
    void            ImportDefaultResourceType(tinyxml2::XMLElement* subtree, string folderName, string extension);
    void            ImportDefaultShaders(tinyxml2::XMLElement* subtree);

    template<typename T>
    void AddResourcesToMap(tinyxml2::XMLElement* resources, string typeName);

    ResourceMap                                  m_resourceMap;
    unordered_map<unsigned int, Resource*>       m_loadedResources;
    unordered_map<string, unsigned int>          m_defaultResources;
    ToolsideComponentSchema*                     m_componentSchema;
    ToolsideShaderSchema*                        m_shaderSchema;

    bool m_lookupTableLoaded;

    string m_resourceBasePath;
};