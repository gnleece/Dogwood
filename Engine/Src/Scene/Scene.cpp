#include "Scene\Scene.h"

#include "Scene\ResourceManager.h"
#include "GameComponentFactory.h"
#include "GameObject.h"
#include "GameObjectBase.h"
#include "GameProject.h"
#include "ToolsideGameComponent.h"
#include "ToolsideGameObject.h"
#include "Util.h"
#include "Physics\Collider.h"
#include "Physics\CollisionEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\Mesh.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\RenderManager.h"
#include "Rendering\Texture.h"
#include "Serialization\HierarchicalSerializer.h"

using namespace tinyxml2;

Scene::Scene()
: m_loaded(false)
{ }

Scene* Scene::New(string filename)
{
    if (!GameProject::Singleton().IsToolside())
    {
        printf("Scene error: can't create new scene in game mode!\n");
        return false;
    }

    Scene* scene = new Scene();

    if (scene != NULL)
    {
        scene->m_filename = filename;
        scene->m_guid = MakeGuid(filename);
        unsigned int guid = MakeGuid("ROOT");
        scene->m_rootObject = new ToolsideGameObject(guid, "ROOT");
        scene->m_loaded = true;

        // TODO more default scene setup
    }

    return scene;
}

Scene* Scene::Load(string filename)
{
    Scene* scene = new Scene();

    if (scene != NULL)
    {
        scene->m_filename = filename;

        printf("LOADING SCENE: %s\n", scene->m_filename.c_str());

        // Open the scene XML file
        XMLDocument sceneDoc;
        XMLError result = sceneDoc.LoadFile(scene->m_filename.c_str());
        if (result != XML_SUCCESS)
        {
            printf("Error reading scene file %s.\nXMLError %d\n", scene->m_filename.c_str(), result);
            return false;
        }
        XMLElement* sceneXML = sceneDoc.FirstChildElement("Scene");

        // Read the guid
        scene->m_guid = sceneXML->UnsignedAttribute("guid");
        if (scene->m_guid == XML_NO_ATTRIBUTE || scene->m_guid == 0)
        {
            scene->m_guid = MakeGuid(scene->m_filename);
        }

        // Load the required resources
        printf("Loading scene resources...\n");
        XMLElement* resources = sceneXML->FirstChildElement("Resources");
        if (resources == NULL)
        {
            printf("Error parsing scene file. Could not find resource list.\n");
            return false;
        }
        ResourceManager::Singleton().LoadSceneResources(resources);

        // Apply global settings (camera, light, etc.)
        scene->DoGlobalSetup(sceneXML);

        // Build the game object hierarchy
        scene->DoHierarchySetup(sceneXML);

        printf("DONE LOADING SCENE!\n");

        scene->m_loaded = true;
    }

    return scene;
}

bool Scene::Save(string filename)
{
    if (!m_loaded)
    {
        printf("Scene error: can't save because no scene is loaded.\n");
        return false;
    }

    if (!GameProject::Singleton().IsToolside())
    {
        printf("Scene error: can't save scene in game mode!\n");
        return false;
    }

    // If a non-empty filename is provided, use it (for "Save As")
    if (filename.compare("") != 0)
    {
        m_filename = filename;
    }

    // Root setup
    HierarchicalSerializer serializer;
    serializer.PushScope("Scene");
    serializer.SetAttribute("guid", m_guid);

    // Track which resources are needed by the scene
    unordered_set<unsigned int> resourceGuids;

    // Serialize body
    SerializeGlobalSettings(&serializer);
    SerializeHierarchy(&serializer, (ToolsideGameObject*)m_rootObject, resourceGuids);
    SerializeResourceList(&serializer, resourceGuids);

    // Save it!
    serializer.Save(m_filename);

    return true;
}

void Scene::Unload(Scene* scene)
{
    if (scene == NULL)
        return;

    if (!scene->m_loaded)
    {
        printf("Scene error: can't unload because scene is not loaded.\n");
    }
    else
    {
        // Unload resources
        ResourceManager::Singleton().UnloadSceneResources();

        // Tear down the hierarchy
        // TODO implement me
    }

    delete scene;
}

GameObjectBase* Scene::GetRootObject()
{
    return m_rootObject;
}

GameObject* Scene::GetRuntimeRootObject()
{
    return (GameObject*)m_rootObject;
}

ToolsideGameObject* Scene::GetToolsideRootObject()
{
    return (ToolsideGameObject*)m_rootObject;
}

bool Scene::IsLoaded()
{
    return m_loaded;
}

void Scene::DoGlobalSetup(XMLElement* sceneXML)
{
    printf("Doing camera & light setup...\n");

    XMLElement* cameraNode = sceneXML->FirstChildElement("Camera");
    if (cameraNode)
    {
        m_mainCamera.position = ReadVector3FromXML(cameraNode->FirstChildElement("Position"));
        m_mainCamera.direction = ReadVector3FromXML(cameraNode->FirstChildElement("Direction"));
        m_mainCamera.up = ReadVector3FromXML(cameraNode->FirstChildElement("Up"));
        RenderManager::Singleton().SetCamera(m_mainCamera);
    }
    else
    {
        printf("Warning: no camera specified in scene file.\n");
    }

    XMLElement* lightNode = sceneXML->FirstChildElement("Light");
    if (lightNode)
    {
        Vector3 lightPosition = ReadVector3FromXML(lightNode->FirstChildElement("Position"));
        ColorRGB lightColor = ReadColorFromXML(lightNode->FirstChildElement("Color"));
        GLfloat lightPower = lightNode->FirstChildElement("Power")->FloatAttribute("value");
        Light light(lightPosition, lightColor, lightPower);
        RenderManager::Singleton().SetLight(light);
        m_light = light;
    }
    else
    {
        printf("Warning: no light specified in scene file.\n");
    }
}

void Scene::DoHierarchySetup(XMLElement* sceneXML)
{
    printf("Building game object hierarchy...\n");

    XMLElement* rootNode = sceneXML->FirstChildElement("GameObject");
    if (rootNode == NULL)
    {
        printf("Warning: no gameobject hierarchy found in scene file!\n");
        return;
    }

    // Process the tree of game objects (recursively, depth first)
    m_rootObject = BuildSubtree(rootNode);
}

GameObjectBase* Scene::BuildSubtree(XMLElement* xmlnode)
{
    if (xmlnode == NULL)
        return NULL;

    // Build & add components on this node
    string name = xmlnode->Attribute("name");
    unsigned int guid = xmlnode->UnsignedAttribute("guid");
    if (guid == 0)
    {
        guid = MakeGuid(name);
    }

    GameObjectBase* go = NULL;
    if (GameProject::Singleton().IsToolside())
    {
        go = new ToolsideGameObject(guid, name);
    }
    else
    {
        go = new GameObject(guid, name);
    }
    AddTransform(go, xmlnode);
    AddMesh(go, xmlnode);
    AddColliders(go, xmlnode);
    AddGameComponents(go, xmlnode);

    // Recursively process child game objects
    XMLElement* childXML = xmlnode->FirstChildElement("GameObject");
    while (childXML)
    {
        GameObjectBase* childgo = BuildSubtree(childXML);
        childgo->SetParent(go);
        childXML = childXML->NextSiblingElement("GameObject");
    }

    return go;
}

void Scene::AddTransform(GameObjectBase* go, XMLElement* xmlnode)
{
    XMLElement* transformXML = xmlnode->FirstChildElement("Transform");
    if (transformXML == NULL)
    {
        return;
    }

    Vector3 position = ReadVector3FromXML(transformXML->FirstChildElement("Position"));
    Vector3 rotation = ReadVector3FromXML(transformXML->FirstChildElement("Rotation"));
    Vector3 scale = ReadVector3FromXML(transformXML->FirstChildElement("Scale"));

    // TODO cleanup with transform functions
    Matrix4x4 matrix = Translation(position);
    matrix = matrix*RotationEulerAngles(rotation);
    matrix = matrix*Scaling(scale);

    go->GetTransform().SetLocalMatrix(matrix);
}

void Scene::AddMesh(GameObjectBase* go, XMLElement* xmlnode)
{
    XMLElement* meshXML = xmlnode->FirstChildElement("Mesh");
    if (meshXML)
    {
        // Find the mesh resource by guid
        unsigned int guid = meshXML->UnsignedAttribute("guid");
        Mesh* mesh = ResourceManager::Singleton().GetMesh(guid);
        if (mesh == NULL)
        {
            printf("Warning: mesh referenced by game object is not loaded\n");
            return;
        }

        // Attach the mesh instance component & do material setup
        MeshInstance* meshInstance = new MeshInstance();
        meshInstance->SetMesh(mesh);
        AddMaterial(meshInstance, meshXML);
        go->SetMesh(meshInstance);
    }
}

void Scene::AddMaterial(MeshInstance* meshInstance, XMLElement* xmlnode)
{
    XMLElement* materialXML = xmlnode->FirstChildElement("Material");
    if (materialXML && meshInstance)
    {
        // Get material component
        Material* material = meshInstance->GetMaterial();

        // Attach shader
        XMLElement* shaderXML = materialXML->FirstChildElement("Shader");
        if (shaderXML)
        {
            unsigned int guid = shaderXML->UnsignedAttribute("guid");
            ShaderProgram* shader = ResourceManager::Singleton().GetShader(guid);
            if (shader == NULL)
            {
                printf("Warning: shader referenced by game object is not loaded\n");
            }
            material->SetShader(shader);
        }

        // Add Colors
        AddMaterialColors(materialXML, material);

        // Add textures
        AddMaterialTextures(materialXML, material);
    }
}

void Scene::AddMaterialColors(XMLElement* xmlnode, Material* material)
{
    XMLElement* colorXML = xmlnode->FirstChildElement("Color");
    while (colorXML)
    {
        string name = colorXML->Attribute("name");
        ColorRGB color = ReadColorFromXML(colorXML);
        material->SetColor(name, color);

        colorXML = colorXML->NextSiblingElement("Color");
    }
}

void Scene::AddMaterialTextures(XMLElement* xmlnode, Material* material)
{
    XMLElement* textureXML = xmlnode->FirstChildElement("Texture");
    while (textureXML)
    {
        unsigned int guid = textureXML->UnsignedAttribute("guid");
        Texture* texture = ResourceManager::Singleton().GetTexture(guid);
        if (texture == NULL)
        {
            printf("Warning: texture referenced by game object is not loaded\n");
            return;
        }
        string name = textureXML->Attribute("name");
        material->SetTexture(name, texture);

        textureXML = textureXML->NextSiblingElement("Texture");
    }
}

void Scene::AddColliders(GameObjectBase* go, tinyxml2::XMLElement* xmlnode)
{
    XMLElement* colliders = xmlnode->FirstChildElement("Colliders");
    if (colliders)
    {
        XMLElement* colliderXML = colliders->FirstChildElement("Collider");
        while (colliderXML)
        {
            Collider* collider = Collider::LoadFromXML(go, colliderXML);
            go->AddCollider(collider);

            if (!GameProject::Singleton().IsToolside())
            {
                CollisionEngine::Singleton().RegisterCollider(collider);
            }

            colliderXML = colliderXML->NextSiblingElement("Collider");
        }
    }
}

void Scene::AddGameComponents(GameObjectBase* go, XMLElement* xmlnode)
{
    XMLElement* gameComponents = xmlnode->FirstChildElement("Components");
    if (gameComponents)
    {
        XMLElement* componentXML = gameComponents->FirstChildElement("Component");
        while (componentXML)
        {
            if (GameProject::Singleton().IsToolside())
            {
                ToolsideGameComponent* component = new ToolsideGameComponent();
                component->Load(componentXML);
                ((ToolsideGameObject*)go)->AddComponent(component);
            }
            else
            {
                unsigned int guid = componentXML->UnsignedAttribute("guid");
                bool isEngineComponent = componentXML->BoolAttribute("engine");
                GameComponentFactory* factory = GameProject::Singleton().GetRuntimeComponentFactory(isEngineComponent);
                GameComponent* component = factory->CreateComponent(guid);
                RuntimeParamList params = ComponentValue::ParseRuntimeParams(componentXML);
                factory->SetParams(guid, component, &params);
                ((GameObject*)go)->AddComponent(component);
            }
            componentXML = componentXML->NextSiblingElement("Component");
        }
    }
}

void Scene::SerializeGlobalSettings(HierarchicalSerializer* serializer)
{
    // Camera
    serializer->PushScope("Camera");
    serializer->InsertLeafVector3("Position",  m_mainCamera.position);
    serializer->InsertLeafVector3("Direction", m_mainCamera.direction);
    serializer->InsertLeafVector3("Up",        m_mainCamera.up);
    serializer->PopScope();

    // Light
    serializer->PushScope("Light");
    serializer->InsertLeafVector3("Position",  m_light.position);
    serializer->InsertLeafColorRGB("Color",    m_light.color);
    serializer->InsertLeaf("Power", "value",   m_light.power);
    serializer->PopScope();
}

void Scene::SerializeHierarchy(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids)
{
    if (gameObject == NULL)
        return;

    // Create node
    serializer->PushScope("GameObject");
    serializer->SetAttribute("guid", gameObject->GetID());
    serializer->SetAttribute("name", gameObject->GetName().c_str());

    // Serialize components
    SerializeTransform(serializer, gameObject);
    SerializeMesh(serializer, gameObject, guids);
    SerializeColliders(serializer, gameObject);
    SerializeComponents(serializer, gameObject, guids);

    // Serialize children
    std::vector<GameObjectBase*>::iterator childIter;
    for (childIter = gameObject->GetChildren().begin(); childIter != gameObject->GetChildren().end(); childIter++)
    {
        ToolsideGameObject* child = (ToolsideGameObject*)*childIter;
        SerializeHierarchy(serializer, child, guids);
    }

    serializer->PopScope();
}
void Scene::SerializeTransform(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject)
{
    if (gameObject == NULL)
        return;

    serializer->PushScope("Transform");
    serializer->InsertLeafVector3("Position",  gameObject->GetTransform().GetLocalPosition());
    serializer->InsertLeafVector3("Rotation",  gameObject->GetTransform().GetLocalRotation());
    serializer->InsertLeafVector3("Scale",     gameObject->GetTransform().GetLocalScale());
    serializer->PopScope();
}

void Scene::SerializeMesh(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids)
{
    if (gameObject == NULL || gameObject->GetMesh() == NULL)
        return;

    serializer->PushScope("Mesh");

    unsigned int guid = gameObject->GetMesh()->GetMesh()->GetResourceInfo()->guid;
    serializer->SetAttribute("guid", guid);
    guids.insert(guid);

    SerializeMaterial(serializer, gameObject, guids);

    serializer->PopScope();
}

void Scene::SerializeMaterial(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids)
{
    if (gameObject == NULL || gameObject->GetMesh() == NULL || gameObject->GetMesh()->GetMaterial() == NULL)
        return;

    Material* material = gameObject->GetMesh()->GetMaterial();

    serializer->PushScope("Material");

    // Serialize shader info
    if (material->GetShader() != NULL)
    {
        serializer->PushScope("Shader");
        unsigned int guid = material->GetShader()->GetResourceInfo()->guid;
        serializer->SetAttribute("guid", guid);
        guids.insert(guid);
        serializer->PopScope();
    }

    // Serialize Color info
    SerializeMaterialColors(serializer, material);

    // Serialize texture info
    SerializeMaterialTextures(serializer, material, guids);

    serializer->PopScope();
}

void Scene::SerializeMaterialColors(HierarchicalSerializer* serializer, Material* material)
{
    unordered_map<GLint, ColorRGB> colors = material->GetColorList();
    unordered_map<GLint, ColorRGB>::iterator iter = colors.begin();

    ShaderProgram* shader = material->GetShader();

    for (; iter != colors.end(); iter++)
    {
        serializer->PushScope("Color");

        string paramName = shader->GetUniformName(iter->first);
        serializer->SetAttribute("name", paramName.c_str());
        serializer->SetAttributeColorRGB(iter->second);

        serializer->PopScope();
    }
}

void Scene::SerializeMaterialTextures(HierarchicalSerializer* serializer, Material* material, unordered_set<unsigned int>& guids)
{
    unordered_map<GLint, Texture*> textures = material->GetTextureList();
    unordered_map<GLint, Texture*>::iterator iter = textures.begin();

    ShaderProgram* shader = material->GetShader();

    for (; iter != textures.end(); iter++)
    {
        serializer->PushScope("Texture");

        unsigned int guid = 0;
        if (iter->second != NULL && iter->second != Texture::DefaultTexture())
        {
            guid = iter->second->GetResourceInfo()->guid;
        }

        serializer->SetAttribute("guid", guid);
        guids.insert(guid);

        string paramName = shader->GetUniformName(iter->first);
        serializer->SetAttribute("name", paramName.c_str());

        serializer->PopScope();
    }
}

void Scene::SerializeColliders(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject)
{
    if (gameObject == NULL)
        return;

    serializer->PushScope("Colliders");

    std::vector<Collider*> colliderList = gameObject->GetColliders();
    std::vector<Collider*>::iterator iter;
    for (iter = colliderList.begin(); iter != colliderList.end(); iter++)
    {
        Collider* collider = *iter;
        collider->Serialize(serializer);
    }

    serializer->PopScope();
}

void Scene::SerializeComponents(HierarchicalSerializer* serializer, ToolsideGameObject* gameObject, unordered_set<unsigned int>& guids)
{
    if (gameObject == NULL)
        return;

    serializer->PushScope("Components");

    std::vector<ToolsideGameComponent*> compList = gameObject->GetComponentList();
    std::vector<ToolsideGameComponent*>::iterator compIter;
    for (compIter = compList.begin(); compIter != compList.end(); compIter++)
    {
        ToolsideGameComponent* comp = *compIter;
        comp->Serialize(serializer, guids);
    }

    serializer->PopScope();
}

void Scene::SerializeResourceList(HierarchicalSerializer* serializer, unordered_set<unsigned int>& guids)
{
    serializer->PushScope("Resources");

    unordered_set<unsigned int>::iterator iter;
    for (iter = guids.begin(); iter != guids.end(); iter++)
    {
        serializer->InsertLeaf("Resource", "guid", *iter);
    }

    serializer->PopScope();
}