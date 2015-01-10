#include "Scene\ResourceManager.h"

#include "Rendering\Mesh.h"
#include "Rendering\Texture.h"

#include <ctime>
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
};

struct ShaderResourceInfo : ResourceInfo
{
    string vertexpath;
    string fragmentpath;

    virtual Resource* Load()
    {
        string absolutePathVert = ResourceManager::Singleton().GetResourceBasePath() + vertexpath;
        string absolutePathFrag = ResourceManager::Singleton().GetResourceBasePath() + fragmentpath;
        ShaderProgram* shader = new ShaderProgram(absolutePathVert, absolutePathFrag, this);
        return shader;
    }

    virtual string TypeName()
    {
        return "Shader";
    }

    void AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map)
    {
        vertexpath = element->Attribute("vertex-path");
        fragmentpath = element->Attribute("fragment-path");
        guid = element->IntAttribute("guid");
        map[guid] = this;
    }

    void Serialize(XMLDocument& rootDoc, XMLElement* parent)
    {
        XMLElement* xml = rootDoc.NewElement(TypeName().c_str());
        xml->SetAttribute("guid", guid);
        xml->SetAttribute("vertex-path", vertexpath.c_str());
        xml->SetAttribute("fragment-path", fragmentpath.c_str());
        parent->InsertEndChild(xml);
    }
};

void ResourceInfo::AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map)
{
    path = element->Attribute("path");
    guid = element->IntAttribute("guid");
    map[guid] = this;
}

void ResourceInfo::Serialize(XMLDocument& rootDoc, XMLElement* parent)
{
    XMLElement* xml = rootDoc.NewElement(TypeName().c_str());
    xml->SetAttribute("guid", guid);
    xml->SetAttribute("path", path.c_str());
    parent->InsertEndChild(xml);
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
    ClearResourceLookupTable();
}

void ResourceManager::LoadResourceMap(XMLElement* resources)
{
    // Unload previous project
    ClearResourceLookupTable();

    // Process each resource type
    AddResourcesToMap<TextureResourceInfo>(resources, "Textures");
    AddResourcesToMap<MeshResourceInfo>(resources, "Meshes");
    AddResourcesToMap<ShaderResourceInfo>(resources, "Shaders");

    m_lookupTableLoaded = true;
}

void ResourceManager::SerializeResourceMap(XMLDocument& rootDoc, XMLElement* parent)
{
    // Create parent node for each resource type
    XMLElement* textureXML = rootDoc.NewElement("Textures");
    XMLElement* meshXML = rootDoc.NewElement("Meshes");
    XMLElement* shaderXML = rootDoc.NewElement("Shaders");
    parent->InsertEndChild(textureXML);
    parent->InsertEndChild(meshXML);
    parent->InsertEndChild(shaderXML);

    // Serialize lookup table
    unordered_map<int, ResourceInfo*>::iterator iter;
    for (iter = m_resourceLookup.begin(); iter != m_resourceLookup.end(); iter++)
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

        if (resourceParent != NULL)
        {
            iter->second->Serialize(rootDoc, resourceParent);
        }
    }
}

bool ResourceManager::ImportResource(string& filepath, string& type)
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
    // TODO handle shaders

    if (resource == NULL)
    {
        return false;
    }

    // Get timestamp
    time_t timer;
    time(&timer);
    string timestamp = std::to_string(timer);

    // Use filepath + timestamp to create guid for this resource
    unsigned int guid = std::hash<string>()(filepath + timestamp);
    resource->path = filepath;
    resource->guid = guid;

    if (m_resourceLookup.count(guid) != 0)
    {
        // Because the guid is a hash of filepath + timestamp, this should basically never happen
        printf("ResourceManager error: trying to add a resource with an exisiting guid.\n");
        return false;
    }

    // TODO also check whether this filepath already exists in the table

    m_resourceLookup[guid] = resource;
    return true;
}

void ResourceManager::LoadSceneResources(XMLElement* resources)
{
    XMLElement* element = resources->FirstChildElement();
    while (element)
    {
        unsigned int guid = element->UnsignedAttribute("guid");
        ResourceInfo* info = m_resourceLookup[guid];
        if (info)
        {
            printf("Loading guid %d\n", guid);
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

Resource* ResourceManager::GetResource(unsigned int guid)
{
    return m_loadedResources[guid];
}

Texture* ResourceManager::GetTexture(unsigned int guid)
{
    return (Texture*)m_loadedResources[guid];
}

Mesh* ResourceManager::GetMesh(unsigned int guid)
{
    return (Mesh*)m_loadedResources[guid];
}

ShaderProgram* ResourceManager::GetShader(unsigned int guid)
{
    return (ShaderProgram*)m_loadedResources[guid];
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

void ResourceManager::ClearResourceLookupTable()
{
    // TODO implement me

    m_lookupTableLoaded = false;
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
            info->AddToMap(element, m_resourceLookup);
            element = element->NextSiblingElement();
        }
    }
}