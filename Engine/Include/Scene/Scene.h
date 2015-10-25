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
class GameObjectBase;
class MeshInstance;
class ToolsideGameObject;

class Scene
{
public:
    Scene();

    bool                New(string filename = "");
    bool                Load(string filename);
    bool                Save(string filename = "");
    bool                Unload();

    GameObjectBase*     GetRootObject();
    GameObject*         GetRuntimeRootObject();
    ToolsideGameObject* GetToolsideRootObject();

    bool                IsLoaded();

private:
    void                DoGlobalSetup(tinyxml2::XMLElement* sceneXML);
    void                DoHierarchySetup(tinyxml2::XMLElement* sceneXML);

    // Scene loading helper functions
    GameObjectBase*     BuildSubtree(tinyxml2::XMLElement* xmlnode);
    void                AddTransform(GameObjectBase* go, tinyxml2::XMLElement* xmlnode);
    void                AddMesh(GameObjectBase* go, tinyxml2::XMLElement* xmlnode);
    void                AddMaterial(MeshInstance* meshInstance, tinyxml2::XMLElement* xmlnode);
    void                AddColliders(GameObjectBase* go, tinyxml2::XMLElement* xmlnode);
    void                AddGameComponents(GameObjectBase* go, tinyxml2::XMLElement* xmlnode);
    void                AddMaterialColors(tinyxml2::XMLElement* xmlnode, Material* material);
    void                AddMaterialTextures(tinyxml2::XMLElement* xmlnode, Material* material);
    
    // Scene saving helper functions
    void                SerializeGlobalSettings(tinyxml2::XMLElement* parentNode, tinyxml2::XMLDocument& rootDoc);
    void                SerializeHierarchy(ToolsideGameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void                SerializeTransform(ToolsideGameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void                SerializeMesh(ToolsideGameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void                SerializeMaterial(ToolsideGameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void                SerializeMaterialColors(Material* material, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void                SerializeMaterialTextures(Material* material, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void                SerializeColliders(ToolsideGameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    void                SerializeComponents(ToolsideGameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids);
    void                SerializeResourceList(unordered_set<unsigned int>& guids, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);

    bool                m_loaded;
    string              m_filename;
    Camera              m_mainCamera;
    Light               m_light;

    GameObjectBase*     m_rootObject = NULL;
};