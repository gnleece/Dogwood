#include "Scene.h"

#include "ResourceManager.h"

void Scene::LoadScene(string filename)
{
    // Open the scene XML file
    XMLDocument sceneDoc;
    XMLError result = sceneDoc.LoadFile(filename.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading scene file %s.\nXMLError %d\n", filename.c_str(), result);
        return;
    }

    // Load the required resources
    XMLElement* resources = sceneDoc.FirstChildElement("Scene")->FirstChildElement("Resources");
    if (resources == NULL)
    {
        printf("Error parsing scene file. Could not find resource list.\n");
        return;
    }
    ResourceManager::Singleton().LoadSceneResources(resources);

    // Build the hierarchy
    // TODO implement me
}

void Scene::UnloadScene()
{
    // Unload resources
    ResourceManager::Singleton().UnloadSceneResources();

    // Teardown the hierachy
    // TODO implement me
}