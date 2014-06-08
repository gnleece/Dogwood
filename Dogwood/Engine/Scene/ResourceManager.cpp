#include "ResourceManager.h"

#include "tinyxml2.h"

using namespace tinyxml2;

void ResourceManager::Startup()
{
    // Load Resources XML file and build resource lookup table
    XMLDocument doc;
    doc.LoadFile("Scenes\\Resources.xml");
}

void ResourceManager::Shutdown()
{}