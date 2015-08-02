#include "GameProject.h"
#include "Scene\ResourceManager.h"

#include <tinyxml2.h>

using namespace tinyxml2;

void GameProject::Startup(bool toolside)
{
    m_toolside = toolside;
    m_loaded = false;
    m_filename = "";
    m_resourceDir = "";
}

void GameProject::Shutdown()
{
    Unload();
}

bool GameProject::IsToolside()
{
    return m_toolside;
}

bool GameProject::New(string name, string filename, string resourcePath)
{
    if (m_loaded)
    {
        printf("GameProject error: can't init because project is already loaded.\n");
        return false;
    }

    m_name = name;
    m_filename = filename;
    m_resourceDir = resourcePath;

    // TODO set default settings

    // Prepare resource map
    ResourceManager::Singleton().ClearResourceMap();
    ResourceManager::Singleton().SetResourceBasePath(m_resourceDir);
    ResourceManager::Singleton().ImportDefaultResources();

    m_loaded = true;
    return true;
}

bool GameProject::Load(string filename)
{
    if (m_loaded)
    {
        printf("GameProject error: can't load because project is already loaded.\n");
        return false;
    }

    m_filename = filename;

    printf("LOADING PROJECT: %s\n", m_filename.c_str());

    // Open the project XML file
    XMLDocument projectDoc;
    XMLError result = projectDoc.LoadFile(m_filename.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading project file %s.\nXMLError %d\n", m_filename.c_str(), result);
        return false;
    }
    XMLElement* projectXML = projectDoc.FirstChildElement("Dogwood-Project");
    m_name = projectXML->Attribute("name");

    // Get project settings
    XMLElement* settingsXML = projectXML->FirstChildElement("Settings");
    LoadSettings(settingsXML);

    // Give the resources list to the ResourceManager
    XMLElement* resourcesXML = projectXML->FirstChildElement("Resources");
    ResourceManager::Singleton().LoadResourceMap(resourcesXML);
    ResourceManager::Singleton().SetResourceBasePath(m_resourceDir);

    // Get the list of scenes
    LoadSceneList();

    // TODO this should only happen in Editor, figure out how to do that properly
    ResourceManager::Singleton().LoadComponentSchema();
    ResourceManager::Singleton().LoadShaderSchema();

    m_loaded = true;
    return true;
}

bool GameProject::Save(string filename)
{
    if (!m_loaded)
    {
        printf("GameProject error: can't save because no project is loaded.\n");
        return false;
    }

    // If a non-empty filename is provided, use it (for "Save As")
    if (filename.compare("") != 0)
    {
        m_filename = filename;
    }

    // Root setup
    XMLDocument projectDoc;
    XMLElement* rootElement = projectDoc.NewElement("Dogwood-Project");
    rootElement->SetAttribute("name", m_name.c_str());
    projectDoc.InsertEndChild(rootElement);

    // Save settings
    SerializeSettings(projectDoc, rootElement);

    // Save resource map
    XMLElement* resourcesXML = projectDoc.NewElement("Resources");
    rootElement->InsertEndChild(resourcesXML);
    ResourceManager::Singleton().SerializeResourceMap(projectDoc, resourcesXML);

    // Save the scene list
    SerilaizeSceneList();

    // Save it!
    projectDoc.SaveFile(m_filename.c_str());
    return true;
}

bool GameProject::Unload()
{
    if (!m_loaded)
    {
        printf("GameProject error: can't unload because project is not loaded.\n");
        return false;
    }

    m_loaded = false;
    return true;
}

bool GameProject::IsLoaded()
{
    return m_loaded;
}

void GameProject::SetRuntimeComponentFactory(GameComponentFactory* factory)
{
    m_componentFactory = factory;
}

GameComponentFactory* GameProject::GetRuntimeComponentFactory()
{
    return m_componentFactory;
}

string GameProject::GetName()
{
    return m_name;
}

void GameProject::SetName(string name)
{
    m_name = name;
}

string GameProject::GetFilename()
{
    return m_filename;
}

void GameProject::GetResolution(int& width, int& height)
{
    width = m_width;
    height = m_height;
}

void GameProject::SetResolution(int width, int height)
{
    m_width = width;
    m_height = height;
}

string GameProject::GetResourceBasePath()
{
    return m_resourceDir;
}

void GameProject::SetResourceBasePath(string path)
{
    // TODO remap asset relative paths? give warning?

    m_resourceDir = path;
}

void GameProject::AddScene(Scene* scene)
{
    // TODO implement me
}

void GameProject::RemoveScene(Scene* scene)
{
    // TODO implement me
}

void GameProject::LoadSettings(XMLElement* settingsXML)
{
    if (settingsXML == NULL)
        return;

    XMLElement* resolutionXML = settingsXML->FirstChildElement("Resolution");
    if (resolutionXML)
    {
        m_width = resolutionXML->IntAttribute("width");
        m_height = resolutionXML->IntAttribute("height");
    }

    XMLElement* resourcePathXML = settingsXML->FirstChildElement("Resource-Root-Path");
    if (resourcePathXML)
    {
        m_resourceDir = resourcePathXML->Attribute("path");
    }
}

void GameProject::LoadSceneList()
{
    // TODO implement me
}

void GameProject::SerializeSettings(XMLDocument& rootDoc, XMLElement* parent)
{
    XMLElement* settingsXML = rootDoc.NewElement("Settings");
    parent->InsertEndChild(settingsXML);

    XMLElement* resolutionXML = rootDoc.NewElement("Resolution");
    resolutionXML->SetAttribute("width", m_width);
    resolutionXML->SetAttribute("height", m_height);
    settingsXML->InsertEndChild(resolutionXML);

    XMLElement* resourcePathXML = rootDoc.NewElement("Resource-Root-Path");
    resourcePathXML->SetAttribute("path", m_resourceDir.c_str());
    settingsXML->InsertEndChild(resourcePathXML);
}

void GameProject::SerilaizeSceneList()
{
    // TODO implement me
}