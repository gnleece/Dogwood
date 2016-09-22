#include "Scene\ResourceManager.h"

#include "GameProject.h"
#include "Util.h"
#include "Rendering\Material.h"
#include "Rendering\Mesh.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\Texture.h"
#include "Serialization\HierarchicalSerializer.h"
#include "ToolsideGameComponent.h"
#include "ToolsideGameObject.h"
#include "ToolsideShaderSchema.h"

#include <algorithm>

bool ResourceInfo::operator<(const ResourceInfo& other) const
{
    return guid < other.guid;
}

struct TextureResourceInfo : ResourceInfo
{ 
    virtual Resource* Load()
    {
        string absolutePath = ResourceManager::Singleton().GetResourceBasePath() + path;
        Texture* texture = new Texture(absolutePath, this);
        return texture;
    }

    virtual string TypeName()
    {
        return "Texture";
    }

    virtual void AddToGameObject(ToolsideGameObject* gameObject)
    {
        // Do nothing here. Textures need to be set as specific material or component param references
        // (i.e. there is no single unique texture per object so we can't simply "add" one to a game object)
    }

    virtual void RemoveFromGameObject(ToolsideGameObject* gameObject)
    {
        // Do nothing.
    }
};

struct MeshResourceInfo : ResourceInfo
{
    virtual Resource* Load()
    {
        string absolutePath = ResourceManager::Singleton().GetResourceBasePath() + path;
        Mesh* mesh = new Mesh(absolutePath, this);
        return mesh;
    }

    virtual string TypeName()
    {
        return "Mesh";
    }

    virtual void AddToGameObject(ToolsideGameObject* gameObject)
    {
        Mesh* mesh = ResourceManager::Singleton().GetMesh(guid);
        MeshInstance* meshInstance = gameObject->GetMeshInstance();
        if (meshInstance != NULL)
        {
            delete meshInstance;
            meshInstance = NULL;
        }
        meshInstance = new MeshInstance();
        meshInstance->SetMesh(mesh);
        gameObject->SetMeshInstance(meshInstance);

        ShaderProgram* shader = (ShaderProgram*)ResourceManager::Singleton().GetDefaultResource("shader_gouraud");
        shader->GetResourceInfo()->AddToGameObject(gameObject);
    }

    virtual void RemoveFromGameObject(ToolsideGameObject* gameObject)
    {
        MeshInstance* meshInstance = gameObject->GetMeshInstance();
        if (meshInstance != NULL)
        {
            bool match = (meshInstance->GetMesh()->GetResourceInfo()->guid == guid);
            if (match)
            {
                delete meshInstance;
            }
        }
    }
};

struct ScriptResourceInfo : ResourceInfo
{
    virtual Resource* Load()
    {
        // TODO as long as the script schema info is stored in one file per project, there's nothing to do here
        return NULL;
    }

    virtual string TypeName()
    {
        return "Script";
    }

    virtual void AddToGameObject(ToolsideGameObject* gameObject)
    {
        if (!GameProject::Singleton().IsToolside())
            return;

        ToolsideGameComponent* component = new ToolsideGameComponent();
        component->Create(guid, false);
        gameObject->AddComponent(component);
    }

    virtual void RemoveFromGameObject(ToolsideGameObject* gameObject)
    {
        // TODO implement me
    }
};

struct ShaderResourceInfo : ResourceInfo
{
    virtual Resource* Load()
    {
        string absolutePath = ResourceManager::Singleton().GetResourceBasePath() + path;
        ShaderProgram* shader = new ShaderProgram(absolutePath, this);
        return shader;
    }

    virtual string TypeName()
    {
        return "Shader";
    }

    virtual void AddToGameObject(ToolsideGameObject* gameObject)
    {
        MeshInstance* meshInstance = gameObject->GetMeshInstance();
        if (meshInstance != NULL)
        {
            Material* mat = meshInstance->GetMaterial();
            if (mat != NULL)
            {
                ShaderProgram* shader = ResourceManager::Singleton().GetShader(guid);
                mat->SetShader(shader);

                ShaderParamList* params = ResourceManager::Singleton().GetShaderParamList(guid);
                ShaderParamList::iterator iter = params->begin();
                for (; iter != params->end(); iter++)
                {
                    ShaderParamType paramType = iter->first;
                    switch (paramType)
                    {
                    case SHADERPARAM_COLOR:
                        mat->SetColor(iter->second, ColorRGB::White);
                        break;
                    case SHADERPARAM_FLOAT:
                        // TODO implement me
                        break;
                    case SHADERPARAM_TEXTURE:
                        mat->SetTexture(iter->second, Texture::DefaultTexture());
                        break;
                    }
                }
            }
        }
    }

    virtual void RemoveFromGameObject(ToolsideGameObject* gameObject)
    {
        MeshInstance* meshInstance = gameObject->GetMeshInstance();
        if (meshInstance != NULL)
        {
            Material* mat = meshInstance->GetMaterial();
            if (mat != NULL)
            {
                bool match = (mat->GetShader()->GetResourceInfo()->guid == guid);
                if (match)
                {
                    mat->SetShader(NULL);
                }
            }
        }
    }
};

void ResourceInfo::AddToMap(HierarchicalDeserializer* deserializer, unordered_map<unsigned int, ResourceInfo*> & map)
{
    deserializer->GetAttribute("guid", guid);
    deserializer->GetAttribute("path", path);
    map[guid] = this;
}

void ResourceInfo::Serialize(HierarchicalSerializer* serializer)
{
    serializer->PushScope(TypeName());
    serializer->SetAttribute("guid", guid);
    serializer->SetAttribute("path", path);
    serializer->PopScope();
}

void ResourceInfo::Unload()
{
    // TODO implement me
}

void ResourceManager::Startup()
{
    m_lookupTableLoaded = false;
}

void ResourceManager::Shutdown()
{
    // Unload currently loaded resources, if any
    UnloadSceneResources();

    // Unload resource map
    ClearResourceMap();
}

void ResourceManager::LoadResourceMap(HierarchicalDeserializer* deserializer)
{
    // Unload previous project
    ClearResourceMap();

    if (deserializer->PushScope("Resources"))
    {

        // Process each resource type
        AddResourcesToMap<TextureResourceInfo>(deserializer, "Textures");
        AddResourcesToMap<MeshResourceInfo>(deserializer, "Meshes");
        AddResourcesToMap<ShaderResourceInfo>(deserializer, "Shaders");
        AddResourcesToMap<ScriptResourceInfo>(deserializer, "Scripts");

        // Load default resource map
        bool success = deserializer->PushScope("DefaultResources");
        if (success)
        {
            bool assetsToProcess = deserializer->PushScope();
            while (assetsToProcess)
            {
                string name;
                deserializer->GetAttribute("name", name);

                unsigned int guid;
                deserializer->GetAttribute("guid", guid);

                m_defaultResources[name] = guid;

                assetsToProcess = deserializer->NextSiblingScope();
            }
            deserializer->PopScope();
        }

        m_lookupTableLoaded = true;

        deserializer->PopScope();
    }
}

void ResourceManager::ClearResourceMap()
{
    UnloadSceneResources();

    unordered_map<unsigned int, ResourceInfo*>::iterator iter;
    for (iter = m_resourceMap.begin(); iter != m_resourceMap.end(); iter++)
    {
        ResourceInfo* info = iter->second;
        delete info;
    }
    m_resourceMap.clear();
    m_defaultResources.clear();

    m_lookupTableLoaded = false;
}

void ResourceManager::SerializeResourceMap(HierarchicalSerializer* serializer)
{
    serializer->PushScope("Resources");

    // Sort the lookup table by type, putting each type into its own list
    vector<ResourceInfo*> textureList;
    vector<ResourceInfo*> meshList;
    vector<ResourceInfo*> shaderList;
    vector<ResourceInfo*> scriptList;

    unordered_map<unsigned int, ResourceInfo*>::iterator iter;
    for (iter = m_resourceMap.begin(); iter != m_resourceMap.end(); iter++)
    {
        if (iter->first == 0)
        {
            // A guid of 0 indicates a "default" resource (e.g. default texture)
            continue;
        }

        if (iter->second == NULL)
        {
            // This occurs for engine components
            // TODO do we need to serialize the list of non-engine components at all?
            continue;
        }

        // TODO this is pretty ugly

        if (strcmp(iter->second->TypeName().c_str(), "Texture") == 0)
        {
            textureList.push_back(iter->second);
        }
        else if (strcmp(iter->second->TypeName().c_str(), "Mesh") == 0)
        {
            meshList.push_back(iter->second);
        }
        else if (strcmp(iter->second->TypeName().c_str(), "Shader") == 0)
        {
            shaderList.push_back(iter->second);
        }
        else if (strcmp(iter->second->TypeName().c_str(), "Script") == 0)
        {
            scriptList.push_back(iter->second);
        }
    }

    SerializeResourceList(serializer, "Textures",   textureList);
    SerializeResourceList(serializer, "Meshes",     meshList);
    SerializeResourceList(serializer, "Shaders",    shaderList);
    SerializeResourceList(serializer, "Scripts",    scriptList);

    // Serialize default resource lookup  // TODO sort based on guid
    serializer->PushScope("DefaultResources");
    unordered_map<string, unsigned int>::iterator defIter;
    for (defIter = m_defaultResources.begin(); defIter != m_defaultResources.end(); defIter++)
    {
        serializer->PushScope("Asset");
        serializer->SetAttribute("name", defIter->first);
        serializer->SetAttribute("guid", defIter->second);
        serializer->PopScope();
    }
    serializer->PopScope();

    serializer->PopScope();
}

void ResourceManager::SerializeResourceList(HierarchicalSerializer* serializer, string name, vector<ResourceInfo*>& resources)
{
    serializer->PushScope(name);

    std::sort(resources.begin(), resources.end(), PComp<ResourceInfo>);
    vector<ResourceInfo*>::iterator iter;
    for (iter = resources.begin(); iter != resources.end(); iter++)
    {
        (*iter)->Serialize(serializer);
    }

    serializer->PopScope();
}

void ResourceManager::LoadComponentSchema()
{
    // Game components
    if (m_componentSchema != NULL)
    {
        delete m_componentSchema;
    }
    m_componentSchema = new ToolsideComponentSchema();
    m_componentSchema->Load(m_resourceBasePath + "ScriptSchema.xml");

    // Engine components
    if (m_engineComponentSchema != NULL)
    {
        delete m_engineComponentSchema;
    }
    m_engineComponentSchema = new ToolsideComponentSchema();
    m_engineComponentSchema->Load(m_resourceBasePath + "EngineScriptSchema.xml");
}

void ResourceManager::LoadShaderSchema()
{
    if (m_shaderSchema == NULL)
    {
        m_shaderSchema = new ToolsideShaderSchema();
    }

    m_shaderSchema->Load(m_resourceBasePath + "ShaderSchema.xml");
}

unsigned int ResourceManager::ImportResource(string& filepath, string type)
{
    ResourceInfo* resource = NULL;

    if (strcmp(type.c_str(), "obj") == 0)
    {
        // Mesh
        resource = new MeshResourceInfo();
    }
    else if (strcmp(type.c_str(), "bmp") == 0)
    {
        // Texture
        resource = new TextureResourceInfo();
    }
    else if (strcmp(type.c_str(), "glsl") == 0)
    {
        // Shader
        resource = new ShaderResourceInfo();
    }
    else if (strcmp(type.c_str(), "h") == 0)
    {
        // Script
        resource = new ScriptResourceInfo();
    }

    if (resource == NULL)
    {
        return 0;
    }

    resource->path = filepath;
    resource->guid = MakeGuid(filepath);

    return Import(resource);
}

unsigned int ResourceManager::Import(ResourceInfo* resource)
{
    if (m_resourceMap.count(resource->guid) != 0)
    {
        // Because the guid is a hash of filepath + timestamp, this should basically never happen
        printf("ResourceManager error: trying to add a resource with an existing guid.\n");
        delete resource;
        return 0;
    }

    // TODO also check whether this filepath already exists in the table

    m_resourceMap[resource->guid] = resource;
    resource->Load();
    return resource->guid;
}

void ResourceManager::ImportDefaultResources()
{
    HierarchicalDeserializer deserializer;
    bool success = deserializer.Load("..\\Engine\\Assets\\DefaultAssets.xml");

    if (!success)
    {
        printf("Error reading default assets file.\n");
        return;
    }
    deserializer.PushScope("Dogwood-Default-Resources");

    // Import each asset, by type
    ImportDefaultResourceType(&deserializer, "Textures", "bmp");
    ImportDefaultResourceType(&deserializer, "Meshes", "obj");
    ImportDefaultResourceType(&deserializer, "Shaders", "glsl");

    deserializer.PopScope();
}

void ResourceManager::ImportDefaultResourceType(HierarchicalDeserializer* deserializer, string typeName, string extension)
{
    if (deserializer->PushScope(typeName))
    {
        bool resourcesToProcess = deserializer->PushScope();
        while (resourcesToProcess)
        {
            // Copy the asset to the project's asset folder
            string srcfile;
            deserializer->GetAttribute("path", srcfile);
            string name;
            deserializer->GetAttribute("name", name);
            string destfile = m_resourceBasePath + typeName + "/" + name + "." + extension;
            FileCopy(srcfile, destfile);

            // Import the asset to the project
            string relativePath = AbsolutePathToProjectPath(destfile);
            unsigned int guid = ImportResource(relativePath, extension);
            m_defaultResources[name] = guid;

            resourcesToProcess = deserializer->NextSiblingScope();
        }
        deserializer->PopScope();
    }
}

void ResourceManager::LoadSceneResources(HierarchicalDeserializer* deserializer)
{
    printf("Loading scene resources...\n");
    bool success = deserializer->PushScope("Resources");
    if (!success)
    {
        printf("Error parsing scene file. Could not find resource list.\n");
        return;
    }

    bool resourcesToProcess = deserializer->PushScope("GameObject");
    while (resourcesToProcess)
    {
        unsigned int guid;
        deserializer->GetAttribute("guid", guid);
        ResourceInfo* info = m_resourceMap[guid];
        if (info)
        {
            printf("Loading guid %u\n", guid);
            m_loadedResources[guid] = info->Load();
        }
        else
        {
            printf("Error! Could not find guid %d in database\n", guid);
        }
        resourcesToProcess = deserializer->PushScope("GameObject");
    }

    deserializer->PopScope();
}

void ResourceManager::UnloadSceneResources()
{
    // TODO implement me
}

Resource* ResourceManager::GetResource(unsigned int guid, bool load)
{
    Resource* resource = m_loadedResources[guid];
    if (resource == NULL && load)
    {
        ResourceInfo* info = m_resourceMap[guid];
        if (info != NULL)
        {
            resource = info->Load();
            m_loadedResources[guid] = resource;
        }
    }
    return resource;
}

Texture* ResourceManager::GetTexture(unsigned int guid, bool load)
{
    if (guid == 0)
        return Texture::DefaultTexture();

    return (Texture*)GetResource(guid, load);
}

Mesh* ResourceManager::GetMesh(unsigned int guid, bool load)
{
    return (Mesh*)GetResource(guid, load);
}

ShaderProgram* ResourceManager::GetShader(unsigned int guid, bool load)
{
    return (ShaderProgram*)GetResource(guid, load);
}

Resource* ResourceManager::GetDefaultResource(string name, bool load)
{
    unsigned int guid = m_defaultResources[name];
    return GetResource(guid, load);
}

ResourceInfo* ResourceManager::GetResourceInfo(unsigned int guid)
{
    return m_resourceMap[guid];
}

void ResourceManager::SetResourceBasePath(string& path)
{
    m_resourceBasePath = path;
}

string ResourceManager::GetResourceBasePath()
{
    return m_resourceBasePath;
}

string ResourceManager::AbsolutePathToProjectPath(string& absolutePath)
{
    int basePathLen = strlen(m_resourceBasePath.c_str());
    int absolutePathLen = strlen(absolutePath.c_str());

    // Check whether base project path is a prefix of the absolute path
    int result = strncmp(absolutePath.c_str(), m_resourceBasePath.c_str(), basePathLen);
    if (result != 0)
    {
        // Base path isn't a prefix, so we can't convert the absolute path
        return "";
    }

    // Return the suffix following the project base path
    string projectPath = absolutePath.substr(basePathLen, absolutePathLen - basePathLen);
    return projectPath;
}

ResourceMap& ResourceManager::GetResourceMap()
{
    return m_resourceMap;
}

ParamList* ResourceManager::GetComponentParamList(unsigned int guid, bool isEngine)
{
    if (isEngine)
    {
        return m_engineComponentSchema->GetDefaultParameterList(guid);
    }
    else
    {
        return m_componentSchema->GetDefaultParameterList(guid);
    }
}

ShaderParamList* ResourceManager::GetShaderParamList(unsigned int guid)
{
    return m_shaderSchema->GetParameterList(guid);
}

template<typename T>
void ResourceManager::AddResourcesToMap(HierarchicalDeserializer* deserializer, string typeName)
{
    bool success = deserializer->PushScope(typeName);
    if (success)
    {
        bool resourcesToProcess = deserializer->PushScope();
        while (resourcesToProcess)
        {
            T* info = new T();
            info->AddToMap(deserializer, m_resourceMap);
            resourcesToProcess = deserializer->NextSiblingScope();
        }
        deserializer->PopScope();
    }
}