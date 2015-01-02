#include "Scene\ResourceManager.h"

#include "GameProject.h"
#include "Rendering\Mesh.h"
#include "Rendering\Texture.h"

#include "tinyxml2.h"

using namespace tinyxml2;

struct TextureResourceInfo : ResourceInfo
{ 
    virtual Resource* Load()
    {
        Texture* texture = new Texture(path, this);
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
        Mesh* mesh = new Mesh(path, this);
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
        ShaderProgram* shader = new ShaderProgram(vertexpath, fragmentpath, this);
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

bool ResourceManager::ImportResource(string filepath, string type)
{
    ResourceInfo* resource = NULL;
    int guid = 1337;        // TODO real guid generation

    if (strcmp(type.c_str(), "obj") == 0)
    {
        // Mesh
        resource = new MeshResourceInfo();
        resource->path = filepath;
        resource->guid = guid;
    }
    else if (strcmp(type.c_str(), "bmp") == 0)
    {
        // Texture
        resource = new TextureResourceInfo();
        resource->path = filepath;
        resource->guid = guid;
    }
    // TODO handle shaders

    if (m_resourceLookup.count(guid) != 0)
    {
        printf("ResourceManager error: trying to add a resource with an exisiting guid.\n");
        return false;
    }
    m_resourceLookup[guid] = resource;
    return true;
}

void ResourceManager::LoadSceneResources(XMLElement* resources)
{
    XMLElement* element = resources->FirstChildElement();
    while (element)
    {
        int guid = element->IntAttribute("guid");
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