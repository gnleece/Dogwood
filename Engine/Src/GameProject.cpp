#include "GameProject.h"
#include "Scene\ResourceManager.h"

#include <tinyxml2.h>

using namespace tinyxml2;

GameProject::GameProject()
 : m_loaded(false)
{
}

bool GameProject::New(string filepath)
{
    if (m_loaded)
    {
        printf("GameProject error: can't init because project is already loaded.\n");
        return false;
    }

    m_filepath = filepath;

    // TODO implement me
    return false;
}

bool GameProject::Load(string filepath)
{
    if (m_loaded)
    {
        printf("GameProject error: can't load because project is already loaded.\n");
        return false;
    }

    m_filepath = filepath;

    printf("LOADING PROJECT: %s\n", m_filepath.c_str());

    // Open the scene XML file
    XMLDocument projectDoc;
    XMLError result = projectDoc.LoadFile(m_filepath.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading project file %s.\nXMLError %d\n", m_filepath.c_str(), result);
        return false;
    }
    XMLElement* projectXML = projectDoc.FirstChildElement("Dogwood-Project");
    m_name = projectXML->Attribute("name");

    // Get project settings
    XMLElement* settingsXML = projectXML->FirstChildElement("Settings");
    LoadSettings(settingsXML);

    // Give the resources list to the ResourceManager
    XMLElement* resourcesXML = projectXML->FirstChildElement("Resources");
    ResourceManager::Singleton().BuildResourceMap(resourcesXML);

    m_loaded = true;
    return true;
}

bool GameProject::Save(string filepath)
{
    if (!m_loaded)
    {
        printf("GameProject error: can't save because no project is loaded.\n");
        return false;
    }

    // TODO implement me
    return false;
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

string GameProject::GetName()
{
    return m_name;
}

void GameProject::SetName(string name)
{
    m_name = name;
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
}