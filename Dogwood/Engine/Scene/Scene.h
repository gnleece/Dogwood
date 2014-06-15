#pragma once

#include <string>
#include <tinyxml2.h>

using std::string;
using namespace tinyxml2;

class Scene
{
public:
    void LoadScene(string filename);
    void UnloadScene();

private:
    void DoGlobalSetup(XMLElement* sceneXML);
    void DoHierarchySetup(XMLElement* sceneXML);
};