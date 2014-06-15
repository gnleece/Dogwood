#include "ResourceManager.h"
#include "..\Rendering\Mesh.h"
#include "..\Rendering\Texture.h"

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

void ResourceManager::Startup()
{
    BuildResourceLookupTable();
}

void ResourceManager::Shutdown()
{
    ClearResourceLookupTable();
}

void ResourceManager::LoadSceneResources(string filepath)
{
    // Load the scene XML file
    XMLDocument sceneDoc;
    XMLError result = sceneDoc.LoadFile(filepath.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading scene file. XMLError %d\n", result);
        return;
    }

    XMLElement* resources = sceneDoc.FirstChildElement("Scene")->FirstChildElement("Resources");
    if (resources == NULL)
    {
        printf("Error parsing scene file. Could not find resource list.\n");
        return;
    }

    // Do load for each resource type
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

void ResourceManager::BuildResourceLookupTable()
{
    // Load the resources XML file
    string resourcesFilepath = "Scenes\\Resources.xml";
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
        printf("Error parsing resources file. Could not find Resources node: %s\n", resourcesFilepath);
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
                m_loadedResources[guid] = info->Load();
                printf("Loaded guid %d of type %s\n", guid, typeName.c_str());
            }
            else
            {
                printf("Error! Could not find guid %d in database\n", guid);
            }
            element = element->NextSiblingElement();
        }
    }
}