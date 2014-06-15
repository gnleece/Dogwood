#include "ResourceManager.h"

#include "tinyxml2.h"

void ResourceInfo::AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map)
{
    path = element->Attribute("path");
    int guid = element->IntAttribute("guid");
    map[guid] = this;
}

Resource* ResourceInfo::Load()
{
    // TODO implement me
    return NULL;
}

struct TextureResourceInfo  : ResourceInfo   { };
struct MeshResourceInfo     : ResourceInfo   { };
struct ShaderResourceInfo   : ResourceInfo   { string vertexpath; string fragmentpath; void AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map); };

void ShaderResourceInfo::AddToMap(XMLElement* element, unordered_map<int, ResourceInfo*> & map)
{
    vertexpath = element->Attribute("vertex-path");
    fragmentpath = element->Attribute("fragment-path");
    int guid = element->IntAttribute("guid");
    map[guid] = this;
}

void ResourceManager::Startup()
{
    LoadResourceLookupTable();
}

void ResourceManager::Shutdown()
{
    UnloadResourceLookupTable();
}

void ResourceManager::LoadSceneResources(string filepath)
{
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

    LoadResourcesOfType(resources, "Textures");
    LoadResourcesOfType(resources, "Meshes");
    LoadResourcesOfType(resources, "Shaders");
}

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

void ResourceManager::LoadResourceLookupTable()
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

    // Process each resource type       TODO clean this up with templates or something
    XMLElement* textures = resourceList->FirstChildElement("Textures");
    if (textures)
    {
        XMLElement* textureElement = textures->FirstChildElement();
        while (textureElement)
        {
            TextureResourceInfo* info = new TextureResourceInfo();
            info->AddToMap(textureElement, m_resourceLookup);
            textureElement = textureElement->NextSiblingElement();
        }
    }

    XMLElement* meshes = resourceList->FirstChildElement("Meshes");
    if (meshes)
    {
        XMLElement* meshElement = meshes->FirstChildElement();
        while (meshElement)
        {
            MeshResourceInfo* info = new MeshResourceInfo();
            info->AddToMap(meshElement, m_resourceLookup);
            meshElement = meshElement->NextSiblingElement();
        }
    }

    XMLElement* shaders = resourceList->FirstChildElement("Shaders");
    if (shaders)
    {
        XMLElement* shaderElement = shaders->FirstChildElement();
        while (shaderElement)
        {
            ShaderResourceInfo* info = new ShaderResourceInfo();
            info->AddToMap(shaderElement, m_resourceLookup);
            shaderElement = shaderElement->NextSiblingElement();
        }
    }
}

void ResourceManager::UnloadResourceLookupTable()
{
    // TODO implement me
}