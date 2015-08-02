#include "Scene\ResourceManager.h"

#include "GameProject.h"
#include "Util.h"
#include "Rendering\Material.h"
#include "Rendering\Mesh.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\Texture.h"
#include "ToolsideGameComponent.h"
#include "ToolsideShaderSchema.h"

#include "tinyxml2.h"

using namespace tinyxml2;

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

    virtual void AddToGameObject(GameObject* gameObject)
    {
        MeshInstance* meshInstance = gameObject->GetMesh();
        if (meshInstance != NULL)
        {
            Material* mat = meshInstance->GetMaterial();
            if (mat != NULL)
            {
                Texture* texture = ResourceManager::Singleton().GetTexture(guid);
                mat->SetTexture(texture);
            }
        }
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

    virtual void AddToGameObject(GameObject* gameObject)
    {
        Mesh* mesh = ResourceManager::Singleton().GetMesh(guid);
        MeshInstance* meshInstance = gameObject->GetMesh();
        if (meshInstance == NULL)
        {
            meshInstance = new MeshInstance();
            gameObject->SetMesh(meshInstance);
        }
        meshInstance->SetMesh(mesh);
        if (meshInstance->GetMaterial() != NULL)
        {
            delete meshInstance->GetMaterial();
        }
        Material* material = new Material();
        meshInstance->SetMaterial(material);

        ShaderProgram* shader = (ShaderProgram*)ResourceManager::Singleton().GetDefaultResource("shader_gouraud");
        shader->GetResourceInfo()->AddToGameObject(gameObject);
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

    virtual void AddToGameObject(GameObject* gameObject)
    {
        if (GameProject::Singleton().IsToolside())
        {
            ToolsideGameComponent* component = new ToolsideGameComponent();
            component->Create(guid);
            gameObject->AddToolsideComponent(component);
        }
        else
        {
            // TODO implement me
            return;
        }
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

    virtual void AddToGameObject(GameObject* gameObject)
    {
        MeshInstance* meshInstance = gameObject->GetMesh();
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
                    mat->SetColor(*iter, ColourRGB::White);
                }
            }
        }
    }
};

void ResourceInfo::AddToMap(XMLElement* element, unordered_map<unsigned int, ResourceInfo*> & map)
{
    path = element->Attribute("path");
    guid = element->UnsignedAttribute("guid");
    map[guid] = this;
}

void ResourceInfo::Serialize(XMLDocument& rootDoc, XMLElement* parent)
{
    XMLElement* xml = rootDoc.NewElement(TypeName().c_str());
    xml->SetAttribute("guid", guid);
    xml->SetAttribute("path", path.c_str());
    parent->InsertEndChild(xml);
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

void ResourceManager::LoadResourceMap(XMLElement* resources)
{
    // Unload previous project
    ClearResourceMap();

    // Process each resource type
    AddResourcesToMap<TextureResourceInfo>(resources, "Textures");
    AddResourcesToMap<MeshResourceInfo>(resources, "Meshes");
    AddResourcesToMap<ShaderResourceInfo>(resources, "Shaders");
    AddResourcesToMap<ScriptResourceInfo>(resources, "Scripts");

    // Load default resource map
    XMLElement* elements = resources->FirstChildElement("DefaultResources");
    if (elements)
    {
        XMLElement* element = elements->FirstChildElement();
        while (element)
        {
            string name = element->Attribute("name");
            unsigned int guid = element->UnsignedAttribute("guid");
            m_defaultResources[name] = guid;
            element = element->NextSiblingElement();
        }
    }

    m_lookupTableLoaded = true;
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

void ResourceManager::SerializeResourceMap(XMLDocument& rootDoc, XMLElement* parent)
{
    // Create parent node for each resource type
    XMLElement* textureXML = rootDoc.NewElement("Textures");
    XMLElement* meshXML = rootDoc.NewElement("Meshes");
    XMLElement* shaderXML = rootDoc.NewElement("Shaders");
    XMLElement* scriptXML = rootDoc.NewElement("Scripts");
    parent->InsertEndChild(textureXML);
    parent->InsertEndChild(meshXML);
    parent->InsertEndChild(shaderXML);
    parent->InsertEndChild(scriptXML);

    // Serialize lookup table
    unordered_map<unsigned int, ResourceInfo*>::iterator iter;
    for (iter = m_resourceMap.begin(); iter != m_resourceMap.end(); iter++)
    {
        // TODO this is pretty ugly
        XMLElement* resourceParent = NULL;
        if (strcmp(iter->second->TypeName().c_str(), "Texture") == 0)
        {
            resourceParent = textureXML;
        }
        else if (strcmp(iter->second->TypeName().c_str(), "Mesh") == 0)
        {
            resourceParent = meshXML;
        }
        else if (strcmp(iter->second->TypeName().c_str(), "Shader") == 0)
        {
            resourceParent = shaderXML;
        }
        else if (strcmp(iter->second->TypeName().c_str(), "Script") == 0)
        {
            resourceParent = scriptXML;
        }

        if (resourceParent != NULL)
        {
            iter->second->Serialize(rootDoc, resourceParent);
        }
    }

    // Serialize default resource lookup
    XMLElement* defaultXML = rootDoc.NewElement("DefaultResources");
    parent->InsertEndChild(defaultXML);
    unordered_map<string, unsigned int>::iterator defIter;
    for (defIter = m_defaultResources.begin(); defIter != m_defaultResources.end(); defIter++)
    {
        XMLElement* xml = rootDoc.NewElement("Asset");
        xml->SetAttribute("name", defIter->first.c_str());
        xml->SetAttribute("guid", defIter->second);
        defaultXML->InsertEndChild(xml);
    }
}

void ResourceManager::LoadComponentSchema()
{
    if (m_componentSchema == NULL)
    {
        m_componentSchema = new ToolsideComponentSchema();
    }

    m_componentSchema->Load(m_resourceBasePath + "ScriptSchema.xml");
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
    XMLDocument assetsDoc;
    XMLError result = assetsDoc.LoadFile("..\\Engine\\Assets\\DefaultAssets.xml");
    if (result != XML_SUCCESS)
    {
        printf("Error reading default assets file.\nXMLError %d\n", result);
    }
    XMLElement* assetsXML = assetsDoc.FirstChildElement("Dogwood-Default-Resources");

    // Import each asset, by type
    XMLElement* meshesXML = assetsXML->FirstChildElement("Meshes");
    ImportDefaultResourceType(meshesXML, "Meshes", "obj");
    XMLElement* texturesXML = assetsXML->FirstChildElement("Textures");
    ImportDefaultResourceType(texturesXML, "Textures", "bmp");
    XMLElement* shadersXML = assetsXML->FirstChildElement("Shaders");
    ImportDefaultResourceType(shadersXML, "Shaders", "glsl");
}

void ResourceManager::ImportDefaultResourceType(tinyxml2::XMLElement* subtree, string folderName, string extension)
{
    if (subtree != NULL)
    {
        XMLElement* resourceXML = subtree->FirstChildElement();
        while (resourceXML != NULL)
        {
            // Copy the asset to the project's asset folder
            string srcfile = resourceXML->Attribute("path");
            string name = resourceXML->Attribute("name");
            string destfile = m_resourceBasePath + folderName + "/" + name + "." + extension;
            FileCopy(srcfile, destfile);

            // Import the asset to the project
            string relativePath = AbsolutePathToProjectPath(destfile);
            unsigned int guid = ImportResource(relativePath, extension);
            m_defaultResources[name] = guid;

            resourceXML = resourceXML->NextSiblingElement();
        }
    }
}

void ResourceManager::LoadSceneResources(XMLElement* resources)
{
    XMLElement* element = resources->FirstChildElement();
    while (element)
    {
        unsigned int guid = element->UnsignedAttribute("guid");
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
        element = element->NextSiblingElement();
    }
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
        resource = m_resourceMap[guid]->Load();
        m_loadedResources[guid] = resource;
    }
    return resource;
}

Texture* ResourceManager::GetTexture(unsigned int guid, bool load)
{
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

ParamList* ResourceManager::GetComponentParamList(unsigned int guid)
{
    return m_componentSchema->GetDefaultParameterList(guid);
}

ShaderParamList* ResourceManager::GetShaderParamList(unsigned int guid)
{
    return m_shaderSchema->GetParameterList(guid);
}

template<typename T>
void ResourceManager::AddResourcesToMap(XMLElement* resources, string typeName)
{
    XMLElement* elements = resources->FirstChildElement(typeName.c_str());
    if (elements)
    {
        XMLElement* element = elements->FirstChildElement();
        while (element)
        {
            T* info = new T();
            info->AddToMap(element, m_resourceMap);
            element = element->NextSiblingElement();
        }
    }
}