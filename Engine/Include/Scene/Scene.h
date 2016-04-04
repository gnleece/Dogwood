#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define QT_NO_OPENGL_ES_2

#include <string>
#include <tinyxml2.h>
#include <unordered_set>

#include "Rendering\Camera.h"
#include "Rendering\Light.h"
#include "Rendering\Material.h"

using std::string;
using std::unordered_set;

class GameObject;
class GameObjectBase;
class HierarchicalSerializer;
class MeshInstance;
class ToolsideGameObject;

class Scene
{
public:
    static Scene*       New(string filename = "");
    static Scene*       Load(string filename);
    static void         Unload(Scene* scene);

    bool                Save(string filename = "");

    GameObjectBase*     GetRootObject();
    GameObject*         GetRuntimeRootObject();
    ToolsideGameObject* GetToolsideRootObject();

    bool                IsLoaded();

private:
    Scene();

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
    void                SerializeGlobalSettings(HierarchicalSerializer* serializer);
    void                SerializeHierarchy(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids);
    void                SerializeTransform(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject);
    void                SerializeMesh(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids);
    void                SerializeMaterial(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids);
    void                SerializeMaterialColors(HierarchicalSerializer* serializer, Material* material);
    void                SerializeMaterialTextures(HierarchicalSerializer* serializer, Material* material, unordered_set<unsigned int>& guids);
    void                SerializeColliders(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject);
    void                SerializeComponents(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids);
    void                SerializeResourceList(HierarchicalSerializer* serializer, unordered_set<unsigned int>& guids);

    unsigned int        m_guid;
    bool                m_loaded;
    string              m_filename;
    Camera              m_mainCamera;
    Light               m_light;

    GameObjectBase*     m_rootObject = NULL;
};