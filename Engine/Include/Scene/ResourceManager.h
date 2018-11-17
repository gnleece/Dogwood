#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Resource.h"
#include "ToolsideGameComponent.h"
#include "ToolsideShaderSchema.h"

class GameObject;
class GameProject;
class HierarchicalDeserializer;
class HierarchicalSerializer;
class Mesh;
class ShaderProgram;
class Texture;
class ToolsideGameObject;

using std::string;
using std::unordered_map;
using std::vector;

class ResourceInfo
{
public:
    virtual void        AddToMap(HierarchicalDeserializer* deserializer, unordered_map<unsigned int, ResourceInfo*> & map);
    virtual void        Serialize(HierarchicalSerializer* serializer);

    virtual Resource*   Load() = 0;
    virtual void        Unload();
    virtual string      TypeName() = 0;
    virtual void        AddToGameObject(ToolsideGameObject* gameObject) = 0;

    bool                operator<(const ResourceInfo& other) const;

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

    void            LoadResourceMap(HierarchicalDeserializer* deserializer);
    void            ClearResourceMap();
    void            SerializeResourceMap(HierarchicalSerializer* serializer);
    void            LoadComponentSchema();
    void            LoadShaderSchema();

    unsigned int    ImportResource(string& filepath, string type);
    void            ImportDefaultResources();

    void            LoadSceneResources(HierarchicalDeserializer* deserializer);
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
    ParamList*      GetComponentParamList(unsigned int guid, bool isEngine);
    ShaderParamList* GetShaderParamList(unsigned int guid);

private:
    unsigned int    Import(ResourceInfo* resource);
    void            ImportDefaultResourceType(HierarchicalDeserializer* deserializer, string typeName, string extension);

    void            SerializeResourceList(HierarchicalSerializer* serializer, string name, vector<ResourceInfo*>& resources);

    template<typename T>
    void AddResourcesToMap(HierarchicalDeserializer* deserializer, string typeName);

    ResourceMap                                  m_resourceMap;
    unordered_map<unsigned int, Resource*>       m_loadedResources;
    unordered_map<string, unsigned int>          m_defaultResources;

    ToolsideComponentSchema*                     m_componentSchema;
    ToolsideComponentSchema*                     m_engineComponentSchema;
    ToolsideShaderSchema*                        m_shaderSchema;

    bool m_lookupTableLoaded;

    string m_resourceBasePath;
};