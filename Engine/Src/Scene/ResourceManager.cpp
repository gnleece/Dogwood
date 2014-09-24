#include "Scene\ResourceManager.h"
#include "Rendering\Mesh.h"
#include "Rendering\Texture.h"

#include "tinyxml2.h"

struct TextureResourceInfo : ResourceInfo
{ 
    virtual Resource* Load()
    {
        Texture* texture = new Texture(path);
        return texture;
    }
};

struct MeshResourceInfo : ResourceInfo
{
    virtual Resource* Load()
    {
        Mesh* mesh = new Mesh(path);
        return mesh;
    }
};

struct ShaderResourceInfo : ResourceInfo
{
    string vertexpath;
    string fragmentpath;

    virtual Resource* Load()
    {
        ShaderProgram* shader = new ShaderProgram(vertexpath, fragmentpath);
        return shader;
    }

    void AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map)
    {
        vertexpath = element->Attribute("vertex-path");
        fragmentpath = element->Attribute("fragment-path");
        int guid = element->IntAttribute("guid");
        map[guid] = this;
    }
};

void ResourceInfo::AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map)
{
    path = element->Attribute("path");
    int guid = element->IntAttribute("guid");
    map[guid] = this;
}

void ResourceManager::Startup(string resourcesFilepath)
{
    BuildResourceLookupTable(resourcesFilepath);
}

void ResourceManager::Shutdown()
{
    ClearResourceLookupTable();
}

void ResourceManager::LoadSceneResources(XMLElement* resources)
{
    LoadResourcesOfType(resources, "Textures");
    LoadResourcesOfType(resources, "Meshes");
    LoadResourcesOfType(resources, "Shaders");
}

void ResourceManager::UnloadSceneResources()
{
    // TODO implement me
}

Resource* ResourceManager::GetResource(int guid)
{
    return m_loadedResources[guid];
}

Texture* ResourceManager::GetTexture(int guid)
{
    return (Texture*)m_loadedResources[guid];
}

Mesh* ResourceManager::GetMesh(int guid)
{
    return (Mesh*)m_loadedResources[guid];
}

ShaderProgram* ResourceManager::GetShader(int guid)
{
    return (ShaderProgram*)m_loadedResources[guid];
}

void ResourceManager::SerializeLoadedResourceMap(XMLElement* parentNode, XMLDocument& rootDoc)
{
    // TODO implement me
}

void ResourceManager::BuildResourceLookupTable(string resourcesFilepath)
{
    // Load the resources XML file
    XMLDocument resourcesDoc;
    XMLError result = resourcesDoc.LoadFile(resourcesFilepath.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading resources file. XMLError %d\n", result);
        return;
    }

    XMLElement* resourceList = resourcesDoc.FirstChildElement("Resources");
    if (resourceList == NULL)
    {
        printf("Error parsing resources file. Could not find Resources node: %s\n", resourcesFilepath.c_str());
        return;
    }

    // Process each resource type
    AddResourcesToMap<TextureResourceInfo>(resourceList, "Textures");
    AddResourcesToMap<MeshResourceInfo>(resourceList, "Meshes");
    AddResourcesToMap<ShaderResourceInfo>(resourceList, "Shaders");
}

void ResourceManager::ClearResourceLookupTable()
{
    // TODO implement me
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
            info->typeName = typeName;
            info->AddToMap(element, m_resourceLookup);
            element = element->NextSiblingElement();
        }
    }
};

void ResourceManager::LoadResourcesOfType(XMLElement* resources, string typeName)
{
    XMLElement* resourceSubtree = resources->FirstChildElement(typeName.c_str());
    if (resourceSubtree)
    {
        XMLElement* element = resourceSubtree->FirstChildElement();
        while (element)
        {
            int guid = element->IntAttribute("guid");
            ResourceInfo* info = m_resourceLookup[guid];
            if (info)
            {
                printf("Loading guid %d of type %s\n", guid, typeName.c_str());
                m_loadedResources[guid] = info->Load();
            }
            else
            {
                printf("Error! Could not find guid %d in database\n", guid);
            }
            element = element->NextSiblingElement();
        }
    }
}