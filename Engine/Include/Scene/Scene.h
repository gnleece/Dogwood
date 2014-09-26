#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define QT_NO_OPENGL_ES_2

#include <string>
#include <tinyxml2.h>

#include "..\Rendering\Camera.h"
#include "..\Rendering\Light.h"
#include "..\Rendering\Material.h"

using std::string;

class GameObject;
class MeshInstance;

class Scene
{
public:
    Scene();
    Scene(string filename);

    void        LoadScene(string filename);
    void        SaveScene(string filename = "");
    void        UnloadScene();

    GameObject* GetRootObject();

private:
    void        DoGlobalSetup(tinyxml2::XMLElement* sceneXML);
    void        DoHierarchySetup(tinyxml2::XMLElement* sceneXML);

    // Scene loading helper functions
    GameObject* BuildSubtree(tinyxml2::XMLElement* xmlnode);
    void        AddTransform(GameObject* go, tinyxml2::XMLElement* xmlnode);
    void        AddMesh(GameObject* go, tinyxml2::XMLElement* xmlnode);
    void        AddMaterial(MeshInstance* meshInstance, tinyxml2::XMLElement* xmlnode);
    void        AddGameComponents(GameObject* go, tinyxml2::XMLElement* xmlnode);
    void        ApplyMaterialColor(tinyxml2::XMLElement* xmlnode, Material* material, string colorName, Material::eMatColourType type, ColourRGB defaultColor);
    
    // Scene saving helper functions
    void        SerializeGlobalSettings(tinyxml2::XMLElement* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeHierarchy(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeTransform(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeMesh(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeMaterial(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeMaterialColor(Material* material, string colorName, Material::eMatColourType type, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument rootDoc);
    void        SerializeComponents(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);

    string      m_filename;
    Camera      m_mainCamera;
    Light       m_light;
    GameObject* m_rootObject = NULL;
};