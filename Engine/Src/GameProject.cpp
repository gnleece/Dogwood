#include "GameProject.h"
#include "Scene\ResourceManager.h"

#include <tinyxml2.h>

using namespace tinyxml2;

bool GameProject::Load(string filepath)
{
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

    return true;
}

bool GameProject::Save(string path)
{
    // TODO implement me
    return false;
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