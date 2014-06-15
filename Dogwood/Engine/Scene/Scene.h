#pragma once

#include <string>
#include <tinyxml2.h>

using std::string;
using namespace tinyxml2;

class GameObject;

class Scene
{
public:
    void LoadScene(string filename);
    void UnloadScene();

    GameObject* GetRootObject();

private:
    void DoGlobalSetup(XMLElement* sceneXML);
    void DoHierarchySetup(XMLElement* sceneXML);

    GameObject* BuildSubtree(XMLElement* xmlnode);

    GameObject* m_rootObject = NULL;
};