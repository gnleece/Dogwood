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

using namespace tinyxml2;

Scene::Scene()
: m_loaded(false)
{ }

bool Scene::New(string filename)
{
    if (!GameProject::Singleton().IsToolside())
    {
        printf("Scene error: can't create new scene in game mode!\n");
        return false;
    }

    if (m_loaded)
    {
        printf("Scene error: can't init new scene because scene is already loaded.\n");
        return false;
    }

    m_filename = filename;
    unsigned int guid = MakeGuid("ROOT");
    m_rootObject = new ToolsideGameObject(guid, "ROOT");

    m_loaded = true;
    return true;
}

bool Scene::Load(string filename)
{
    if (m_loaded)
    {
        printf("Scene error: can't load because scene is already loaded.\n");
        return false;
    }

    m_filename = filename;

    printf("LOADING SCENE: %s\n", m_filename.c_str());

    // Open the scene XML file
    XMLDocument sceneDoc;
    XMLError result = sceneDoc.LoadFile(m_filename.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading scene file %s.\nXMLError %d\n", m_filename.c_str(), result);
        return false;
    }
    XMLElement* sceneXML = sceneDoc.FirstChildElement("Scene");

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
    DoGlobalSetup(sceneXML);

    // Build the game object hierarchy
    DoHierarchySetup(sceneXML);

    printf("DONE LOADING SCENE!\n");

    m_loaded = true;
    return true;
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
    XMLDocument sceneDoc;
    XMLElement* rootElement = sceneDoc.NewElement("Scene");
    sceneDoc.InsertEndChild(rootElement);

    // Track which resources are needed by the scene
    unordered_set<unsigned int> resourceGuids;

    // Serialize body
    SerializeGlobalSettings(rootElement, sceneDoc);
    SerializeHierarchy((ToolsideGameObject*)m_rootObject, rootElement, sceneDoc, resourceGuids);
    SerializeResourceList(resourceGuids, rootElement, sceneDoc);

    // Save it!
    sceneDoc.SaveFile(m_filename.c_str());

    return true;
}

bool Scene::Unload()
{
    if (!m_loaded)
    {
        printf("Scene error: can't unload because scene is not loaded.\n");
        return false;
    }

    // Unload resources
    ResourceManager::Singleton().UnloadSceneResources();

    // Tear down the hierarchy
    // TODO implement me

    m_rootObject = NULL;
    m_loaded = false;

    return true;
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
                GameComponentFactory* factory = GameProject::Singleton().GetRuntimeComponentFactory();
                GameComponent* component = factory->CreateComponent(guid);
                RuntimeParamList params = ComponentValue::ParseRuntimeParams(componentXML);
                factory->SetParams(guid, component, &params);
                ((GameObject*)go)->AddComponent(component);
            }
            componentXML = componentXML->NextSiblingElement("Component");
        }
    }
}

void Scene::SerializeGlobalSettings(XMLElement* parentNode, XMLDocument& rootDoc)
{
    // Camera
    XMLNode* cameraNode = parentNode->InsertEndChild(rootDoc.NewElement("Camera"));
    cameraNode->InsertEndChild(WriteVector3ToXML(m_mainCamera.position, "Position", rootDoc));
    cameraNode->InsertEndChild(WriteVector3ToXML(m_mainCamera.direction, "Direction", rootDoc));
    cameraNode->InsertEndChild(WriteVector3ToXML(m_mainCamera.up, "Up", rootDoc));

    // Light
    XMLNode* lightNode = parentNode->InsertEndChild(rootDoc.NewElement("Light"));
    lightNode->InsertEndChild(WriteVector3ToXML(m_light.position, "Position", rootDoc));
    lightNode->InsertEndChild(WriteColorToXML(m_light.color, "Color", rootDoc));
    lightNode->InsertEndChild(WriteFloatToXML(m_light.power, "Power", "value", rootDoc));
}

void Scene::SerializeHierarchy(ToolsideGameObject* gameObject, XMLNode* parentNode, XMLDocument& rootDoc, unordered_set<unsigned int>& guids)
{
    if (gameObject == NULL)
        return;

    // Create node
    XMLElement* goXML = rootDoc.NewElement("GameObject");
    goXML->SetAttribute("guid", gameObject->GetID());
    goXML->SetAttribute("name", gameObject->GetName().c_str());
    parentNode->InsertEndChild(goXML);

    // Serialize components
    SerializeTransform(gameObject, goXML, rootDoc);
    SerializeMesh(gameObject, goXML, rootDoc, guids);
    SerializeColliders(gameObject, goXML, rootDoc);
    SerializeComponents(gameObject, goXML, rootDoc, guids);

    // Serialize children
    std::vector<GameObjectBase*>::iterator childIter;
    for (childIter = gameObject->GetChildren().begin(); childIter != gameObject->GetChildren().end(); childIter++)
    {
        ToolsideGameObject* child = (ToolsideGameObject*)*childIter;
        SerializeHierarchy(child, goXML, rootDoc, guids);
    }
}
void Scene::SerializeTransform(ToolsideGameObject* gameObject, XMLNode* parentNode, XMLDocument& rootDoc)
{
    if (gameObject == NULL)
        return;

    XMLNode* transformNode = parentNode->InsertEndChild(rootDoc.NewElement("Transform"));
    transformNode->InsertEndChild(WriteVector3ToXML(gameObject->GetTransform().GetLocalPosition(), "Position", rootDoc));
    transformNode->InsertEndChild(WriteVector3ToXML(gameObject->GetTransform().GetLocalRotation(), "Rotation", rootDoc));
    transformNode->InsertEndChild(WriteVector3ToXML(gameObject->GetTransform().GetLocalScale(), "Scale", rootDoc));
}

void Scene::SerializeMesh(ToolsideGameObject* gameObject, XMLNode* parentNode, XMLDocument& rootDoc, unordered_set<unsigned int>& guids)
{
    if (gameObject == NULL || gameObject->GetMesh() == NULL)
        return;

    XMLElement* meshNode = rootDoc.NewElement("Mesh");
    unsigned int guid = gameObject->GetMesh()->GetMesh()->GetResourceInfo()->guid;
    meshNode->SetAttribute("guid", guid);
    guids.insert(guid);
    parentNode->InsertEndChild(meshNode);
    SerializeMaterial(gameObject, meshNode, rootDoc, guids);
}

void Scene::SerializeMaterial(ToolsideGameObject* gameObject, XMLNode* parentNode, XMLDocument& rootDoc, unordered_set<unsigned int>& guids)
{
    if (gameObject == NULL || gameObject->GetMesh() == NULL || gameObject->GetMesh()->GetMaterial() == NULL)
        return;

    Material* mat = gameObject->GetMesh()->GetMaterial();

    XMLElement* matNode = rootDoc.NewElement("Material");
    parentNode->InsertEndChild(matNode);

    // Serialize shader info
    if (mat->GetShader() != NULL)
    {
        XMLElement* shaderNode = rootDoc.NewElement("Shader");
        unsigned int guid = mat->GetShader()->GetResourceInfo()->guid;
        shaderNode->SetAttribute("guid", guid);
        guids.insert(guid);
        matNode->InsertEndChild(shaderNode);
    }

    // Serialize Color info
    SerializeMaterialColors(mat, matNode, rootDoc);

    // Serialize texture info
    SerializeMaterialTextures(mat, matNode, rootDoc, guids);
}

void Scene::SerializeMaterialColors(Material* material, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    unordered_map<GLint, ColorRGB> colors = material->GetColorList();
    unordered_map<GLint, ColorRGB>::iterator iter = colors.begin();

    ShaderProgram* shader = material->GetShader();

    for (; iter != colors.end(); iter++)
    {
        XMLElement* node = (WriteColorToXML(iter->second, "Color", rootDoc));
        string paramName = shader->GetUniformName(iter->first);
        node->SetAttribute("name", paramName.c_str());
        parentNode->InsertEndChild(node);
    }
}

void Scene::SerializeMaterialTextures(Material* material, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids)
{
    unordered_map<GLint, Texture*> textures = material->GetTextureList();
    unordered_map<GLint, Texture*>::iterator iter = textures.begin();

    ShaderProgram* shader = material->GetShader();

    for (; iter != textures.end(); iter++)
    {
        XMLElement* textureNode = rootDoc.NewElement("Texture");

        unsigned int guid = 0;
        if (iter->second != NULL && iter->second != Texture::DefaultTexture())
        {
            guid = iter->second->GetResourceInfo()->guid;
        }

        textureNode->SetAttribute("guid", guid);
        guids.insert(guid);

        string paramName = shader->GetUniformName(iter->first);
        textureNode->SetAttribute("name", paramName.c_str());

        parentNode->InsertEndChild(textureNode);
    }
}

void Scene::SerializeColliders(ToolsideGameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    if (gameObject == NULL)
        return;

    XMLElement* collidersNode = rootDoc.NewElement("Colliders");
    parentNode->InsertEndChild(collidersNode);

    std::vector<Collider*> colliderList = gameObject->GetColliders();
    std::vector<Collider*>::iterator iter;
    for (iter = colliderList.begin(); iter != colliderList.end(); iter++)
    {
        Collider* collider = *iter;
        collider->Serialize(collidersNode, rootDoc);
    }
}

void Scene::SerializeComponents(ToolsideGameObject* gameObject, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc, unordered_set<unsigned int>& guids)
{
    if (gameObject == NULL)
        return;

    XMLElement* componentsNode = rootDoc.NewElement("Components");
    parentNode->InsertEndChild(componentsNode);

    std::vector<ToolsideGameComponent*> compList = gameObject->GetComponentList();
    std::vector<ToolsideGameComponent*>::iterator compIter;
    for (compIter = compList.begin(); compIter != compList.end(); compIter++)
    {
        ToolsideGameComponent* comp = *compIter;
        comp->Serialize(componentsNode, rootDoc, guids);
    }
}

void Scene::SerializeResourceList(unordered_set<unsigned int>& guids, tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* resourcesNode = rootDoc.NewElement("Resources");
    parentNode->InsertFirstChild(resourcesNode);

    unordered_set<unsigned int>::iterator iter;
    for (iter = guids.begin(); iter != guids.end(); iter++)
    {
        resourcesNode->InsertEndChild(WriteUnsignedIntToXML(*iter, "Resource", "guid", rootDoc));
    }
}