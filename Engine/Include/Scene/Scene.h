#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define QT_NO_OPENGL_ES_2

#include <string>
#include <unordered_set>

#include "Rendering\Camera.h"
#include "Rendering\Light.h"
#include "Rendering\Material.h"

using std::string;
using std::unordered_set;

class GameObject;
class GameObjectBase;
class HierarchicalDeserializer;
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

    // Scene saving helper functions
    void                SaveGlobalSettings(HierarchicalSerializer* serializer);
    void                SaveHierarchy(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids);
    void                SaveTransform(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject);
    void                SaveMesh(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids);
    void                SaveMaterial(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids);
    void                SaveMaterialColors(HierarchicalSerializer* serializer, Material* material);
    void                SaveMaterialTextures(HierarchicalSerializer* serializer, Material* material, unordered_set<unsigned int>& guids);
    void                SaveColliders(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject);
    void                SaveRigidBodies(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject);
    void                SaveComponents(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids);
    void                SaveResourceList(HierarchicalSerializer* serializer, unordered_set<unsigned int>& guids);

    // Scene loading helper functions
    void                LoadGlobalSettings(HierarchicalDeserializer* deserializer);
    void                LoadHierarchy(HierarchicalDeserializer* deserializer);
    GameObjectBase*     LoadHierarchySubtree(HierarchicalDeserializer* deserializer);
    void                LoadTransform(HierarchicalDeserializer* deserializer, GameObjectBase* go);
    void                LoadMesh(HierarchicalDeserializer* deserializer, GameObjectBase* go);
    void                LoadMaterial(HierarchicalDeserializer* deserializer, MeshInstance* meshInstance);
    void                LoadColliders(HierarchicalDeserializer* deserializer, GameObjectBase* go);
    void                LoadRigidBodies(HierarchicalDeserializer* deserializer, GameObjectBase* go);
    void                LoadGameComponents(HierarchicalDeserializer* deserializer, GameObjectBase* go);
    void                LoadMaterialColors(HierarchicalDeserializer* deserializer, Material* material);
    void                LoadMaterialTextures(HierarchicalDeserializer* deserializer, Material* material);


    unsigned int        m_guid;
    bool                m_loaded;
    string              m_filename;
    Camera              m_mainCamera;
    Light               m_light;

    GameObjectBase*     m_rootObject = NULL;
};