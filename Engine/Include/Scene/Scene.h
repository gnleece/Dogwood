#pragma once

#include <string>
#include <tinyxml2.h>

#include "..\Rendering\Material.h"

using std::string;
using namespace tinyxml2;

class GameObject;
class MeshInstance;

class Scene
{
public:
    Scene();
    Scene(string filename);

    void LoadScene(string filename);
    void UnloadScene();

    GameObject* GetRootObject();

private:
    void DoGlobalSetup(XMLElement* sceneXML);
    void DoHierarchySetup(XMLElement* sceneXML);

    GameObject* BuildSubtree(XMLElement* xmlnode);
    void AddTransform(GameObject* go, XMLElement* xmlnode);
    void AddMesh(GameObject* go, XMLElement* xmlnode);
    void AddMaterial(MeshInstance* meshInstance, XMLElement* xmlnode);
    void AddGameComponents(GameObject* go, XMLElement* xmlnode);

    void ApplyMaterialColor(XMLElement* xmlnode, Material* material, string colorName, Material::eMatColourType type, ColourRGB defaultColor);

    GameObject* m_rootObject = NULL;
};