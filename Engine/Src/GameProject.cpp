#include "GameProject.h"
#include "Scene\ResourceManager.h"
#include "Serialization\HierarchicalSerializer.h"

#include <algorithm>

GameProject::PhysicsSettings::PhysicsSettings()
{
    Enabled = true;
    Gravity = -2.0f;
}

GameProject::PhysicsSettings::PhysicsSettings(bool enabled, float gravity)
{
    Enabled = enabled;
    Gravity = gravity;
}

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

    // Open the project file
    HierarchicalDeserializer deserializer;
    bool success = deserializer.Load(m_filename);
    if (!success)
    {
        printf("Error reading project file %s.\n", m_filename.c_str());
        return false;
    }

    deserializer.PushScope("Dogwood-Project");
    deserializer.GetAttribute("name", m_name);

    // Get project settings
    LoadSettings(&deserializer);

    // Give the resources list to the ResourceManager
    ResourceManager::Singleton().LoadResourceMap(&deserializer);
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
    HierarchicalSerializer serializer;
    serializer.PushScope("Dogwood-Project");
    serializer.SetAttribute("name", m_name);

    // Save settings
    SerializeSettings(&serializer);

    // Save resource map
    ResourceManager::Singleton().SerializeResourceMap(&serializer);

    // Save the scene list
    SerilaizeSceneList();

    // Save it!
    serializer.Save(m_filename);
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

void GameProject::SetRuntimeComponentFactory(GameComponentFactory* factory, bool engine)
{
    if (engine)
    {
        m_engineComponentFactory = factory;
    }
    else
    {
        m_gameComponentFactory = factory;
    }
}

GameComponentFactory* GameProject::GetRuntimeComponentFactory(bool engine)
{
    return engine ? m_engineComponentFactory : m_gameComponentFactory;
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
    // TODO check for dupes
    m_sceneList.push_back(scene);
}

void GameProject::RemoveScene(Scene* scene)
{
    m_sceneList.erase(
        std::remove(m_sceneList.begin(), m_sceneList.end(), scene),
        m_sceneList.end());
}

GameProject::PhysicsSettings& GameProject::GetPhysicsSettings()
{
    return m_physicsSettings;
}

void GameProject::SetPhysicsSettings(PhysicsSettings& settings)
{
    m_physicsSettings = settings;
}

void GameProject::LoadSettings(HierarchicalDeserializer* deserializer)
{
    if (deserializer->PushScope("Settings"))
    {
        if (deserializer->PushScope("Resolution"))
        {
            deserializer->GetAttribute("width", m_width);
            deserializer->GetAttribute("height", m_height);
            deserializer->PopScope();
        }

        if (deserializer->PushScope("Resource-Root-Path"))
        {
            deserializer->GetAttribute("path", m_resourceDir);
            deserializer->PopScope();
        }

        if (deserializer->PushScope("Physics-Settings"))
        {
            deserializer->GetAttribute("enabled", m_physicsSettings.Enabled);
            deserializer->GetAttribute("gravity", m_physicsSettings.Gravity);
            deserializer->PopScope();
        }

        deserializer->PopScope();
    }
}

void GameProject::LoadSceneList()
{
    // TODO implement me
}

void GameProject::SerializeSettings(HierarchicalSerializer* serializer)
{
    serializer->PushScope("Settings");

    serializer->PushScope("Resolution");
    serializer->SetAttribute("width", m_width);
    serializer->SetAttribute("height", m_height);
    serializer->PopScope();

    serializer->PushScope("Resource-Root-Path");
    serializer->SetAttribute("path", m_resourceDir);
    serializer->PopScope();

    serializer->PushScope("Physics-Settings");
    serializer->SetAttribute("enabled", m_physicsSettings.Enabled);
    serializer->SetAttribute("gravity", m_physicsSettings.Gravity);
    serializer->PopScope();

    serializer->PopScope();
}

void GameProject::SerilaizeSceneList()
{
    // TODO implement me
}