#include "Scene.h"

#include "ResourceManager.h"
#include "..\GameObject.h"
#include "..\Util.h"
#include "..\Rendering\Camera.h"
#include "..\Rendering\RenderManager.h"

void Scene::LoadScene(string filename)
{
    printf("LOADING SCENE: %s\n", filename.c_str());

    // Open the scene XML file
    XMLDocument sceneDoc;
    XMLError result = sceneDoc.LoadFile(filename.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading scene file %s.\nXMLError %d\n", filename.c_str(), result);
        return;
    }
    XMLElement* sceneXML = sceneDoc.FirstChildElement("Scene");

    // Load the required resources
    printf("Loading scene resources...\n");
    XMLElement* resources = sceneXML->FirstChildElement("Resources");
    if (resources == NULL)
    {
        printf("Error parsing scene file. Could not find resource list.\n");
        return;
    }
    ResourceManager::Singleton().LoadSceneResources(resources);

    // Apply global settings (camera, light, etc.)
    DoGlobalSetup(sceneXML);

    // Build the game object hierarchy
    DoHierarchySetup(sceneXML);

    printf("DONE LOADING SCENE!\n");
}

void Scene::UnloadScene()
{
    // Unload resources
    ResourceManager::Singleton().UnloadSceneResources();

    // Tear down the hierachy
    // TODO implement me

    m_rootObject = NULL;
}

GameObject* Scene::GetRootObject()
{
    return m_rootObject;
}

void Scene::DoGlobalSetup(XMLElement* sceneXML)
{
    printf("Doing camera & light setup...\n");

    XMLElement* cameraNode = sceneXML->FirstChildElement("Camera");
    if (cameraNode)
    {
        Camera mainCamera;
        mainCamera.position = ReadVector3FromXML(cameraNode->FirstChildElement("Position"));
        mainCamera.direction = ReadVector3FromXML(cameraNode->FirstChildElement("Direction"));
        mainCamera.up = ReadVector3FromXML(cameraNode->FirstChildElement("Up"));
        RenderManager::Singleton().SetCamera(mainCamera);
    }
    else
    {
        printf("Warning: no camera specified in scene file.\n");
    }

    XMLElement* lightNode = sceneXML->FirstChildElement("Light");
    if (lightNode)
    {
        Vector3 lightPosition = ReadVector3FromXML(lightNode->FirstChildElement("Position"));
        ColourRGB lightColor = ReadColourFromXML(lightNode->FirstChildElement("Color"));
        GLfloat lightPower = lightNode->FirstChildElement("Power")->FloatAttribute("value");
        Light light(lightPosition, lightColor, lightPower);
        RenderManager::Singleton().SetLight(light);
    }
    else
    {
        printf("Warning: no light specified in scene file.\n");
    }
}

void Scene::DoHierarchySetup(XMLElement* sceneXML)
{
    printf("Building game object hierarchy...\n");

    XMLElement* rootNode = sceneXML->FirstChildElement("GameObject");
    if (rootNode == NULL)
    {
        printf("Warning: no gameobject hierarchy found in scene file!\n");
        return;
    }

    // Process the tree of game objects (recursively, depth first)
    m_rootObject = BuildSubtree(rootNode);
}

GameObject* Scene::BuildSubtree(XMLElement* xmlnode)
{
    if (xmlnode == NULL)
    {
        return NULL;
    }

    GameObject* go = new GameObject();

    // Build & add components on this node
    // TODO transform, mesh, material, shader, texture, etc
    const char* name = xmlnode->Attribute("name");
    if (name)
    {
        go->SetName(name);
    }

    // Recursively process child game objects
    XMLElement* childxml = xmlnode->FirstChildElement("GameObject");
    while (childxml)
    {
        GameObject* childgo = BuildSubtree(childxml);
        childgo->SetParent(go);
        childxml = childxml->NextSiblingElement("GameObject");
    }

    return go;
}