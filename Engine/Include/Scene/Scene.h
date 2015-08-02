#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define QT_NO_OPENGL_ES_2

#include <string>
#include <tinyxml2.h>
#include <unordered_set>

#include "..\Rendering\Camera.h"
#include "..\Rendering\Light.h"
#include "..\Rendering\Material.h"

using std::string;
using std::unordered_set;

class GameObject;
class MeshInstance;

class Scene
{
public:
    Scene();

    bool        New(string filename = "");
    bool        Load(string filename);
    bool        Save(string filename = "");
    bool        Unload();

    GameObject* GetRootObject();
    bool        IsLoaded();

private:
    void        DoGlobalSetup(tinyxml2::XMLElement* sceneXML);
    void        DoHierarchySetup(tinyxml2::XMLElement* sceneXML);

    // Scene loading helper functions
    GameObject* BuildSubtree(tinyxml2::XMLElement* xmlnode);
    void        AddTransform(GameObject* go, tinyxml2::XMLElement* xmlnode);
    void        AddMesh(GameObject* go, tinyxml2::XMLElement* xmlnode);
    void        AddMaterial(MeshInstance* meshInstance, tinyxml2::XMLElement* xmlnode);
    void        AddGameComponents(GameObject* go, tinyxml2::XMLElement* xmlnode);
    void        AddMaterialColors(tinyxml2::XMLElement* xmlnode, Material* material);
    void        AddMaterialTextures(tinyxml2::XMLElement* xmlnode, Material* material);
    
    // Scene saving helper functions
    void        SerializeGlobalSettings(tinyxml2::XMLElement* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeHierarchy(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void        SerializeTransform(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeMesh(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void        SerializeMaterial(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void        SerializeMaterialColors(Material* material, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeMaterialTextures(Material* material, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void        SerializeComponents(GameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void        SerializeResourceList(unordered_set<unsigned int>& guids, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);

    bool        m_loaded;
    string      m_filename;
    Camera      m_mainCamera;
    Light       m_light;
    GameObject* m_rootObject = NULL;
};