#include "Scene\Scene.h"

#include "Scene\ResourceManager.h"
#include "GameObject.h"
#include "Util.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\RenderManager.h"
//#include "..\..\Generated\GameComponentBindings.h"

Scene::Scene()
{ }

Scene::Scene(string filename)
{
    LoadScene(filename);
}

void Scene::LoadScene(string filename)
{
    m_filename = filename;

    printf("LOADING SCENE: %s\n", m_filename.c_str());

    // Open the scene XML file
    XMLDocument sceneDoc;
    XMLError result = sceneDoc.LoadFile(m_filename.c_str());
    if (result != XML_SUCCESS)
    {
        printf("Error reading scene file %s.\nXMLError %d\n", m_filename.c_str(), result);
        return;
    }
    XMLElement* sceneXML = sceneDoc.FirstChildElement("Scene");

    // Load the required resources
    printf("Loading scene resources...\n");
    XMLElement* resources = sceneXML->FirstChildElement("Resources");
    if (resources == NULL)
    {
        printf("Error parsing scene file. Could not find resource list.\n");
        return;
    }
    ResourceManager::Singleton().LoadSceneResources(resources);

    // Apply global settings (camera, light, etc.)
    DoGlobalSetup(sceneXML);

    // Build the game object hierarchy
    DoHierarchySetup(sceneXML);

    printf("DONE LOADING SCENE!\n");
}

void Scene::SaveScene(string filename)
{
    // If a non-empty filename is provided, use it (for "Save As")
    if (filename.compare("") != 0)
    {
        m_filename = filename;
    }

    // Root setup
    XMLDocument sceneDoc;
    XMLElement* rootElement = sceneDoc.NewElement("Scene");
    sceneDoc.InsertEndChild(rootElement);

    // Serialize body
    SerializeGlobalSettings(rootElement, sceneDoc);
    ResourceManager::Singleton().SerializeLoadedResourceMap(rootElement, sceneDoc);
    SerializeHierarchy(rootElement, sceneDoc);

    // Save it!
    sceneDoc.SaveFile("save_test.xml");
    //sceneDoc.SaveFile(m_filename.c_str());
}

void Scene::UnloadScene()
{
    // Unload resources
    ResourceManager::Singleton().UnloadSceneResources();

    // Tear down the hierachy
    // TODO implement me

    m_rootObject = NULL;
}

GameObject* Scene::GetRootObject()
{
    return m_rootObject;
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
        ColourRGB lightColor = ReadColourFromXML(lightNode->FirstChildElement("Color"));
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

GameObject* Scene::BuildSubtree(XMLElement* xmlnode)
{
    if (xmlnode == NULL)
        return NULL;

    // Build & add components on this node
    GameObject* go = new GameObject();
    go->SetName(xmlnode->Attribute("name"));
    AddTransform(go, xmlnode);
    AddMesh(go, xmlnode);
    AddGameComponents(go, xmlnode);

    // Recursively process child game objects
    XMLElement* childXML = xmlnode->FirstChildElement("GameObject");
    while (childXML)
    {
        GameObject* childgo = BuildSubtree(childXML);
        childgo->SetParent(go);
        childXML = childXML->NextSiblingElement("GameObject");
    }

    return go;
}

void Scene::AddTransform(GameObject* go, XMLElement* xmlnode)
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

    go->SetLocalTransform(matrix);
}

void Scene::AddMesh(GameObject* go, XMLElement* xmlnode)
{
    XMLElement* meshXML = xmlnode->FirstChildElement("Mesh");
    if (meshXML)
    {
        // Find the mesh resource by guid
        int guid = meshXML->IntAttribute("guid");
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
        // Create material component
        Material* material = new Material();
        meshInstance->SetMaterial(material);

        // Attach shader
        XMLElement* shaderXML = materialXML->FirstChildElement("Shader");
        if (shaderXML)
        {
            int guid = shaderXML->IntAttribute("guid");
            ShaderProgram* shader = ResourceManager::Singleton().GetShader(guid);
            if (shader == NULL)
            {
                printf("Warning: shader referenced by game object is not loaded\n");
            }
            material->SetShader(shader);
        }

        // Attach texture
        XMLElement* textureXML = materialXML->FirstChildElement("Texture");
        if (textureXML)
        {
            int guid = textureXML->IntAttribute("guid");
            Texture* texture = ResourceManager::Singleton().GetTexture(guid);
            if (texture == NULL)
            {
                printf("Warning: texture referenced by game object is not loaded\n");
                return;
            }
            material->SetTexture(texture);
        }

        // Apply colours
        ApplyMaterialColor(materialXML, material, "ColorDiffuse", Material::MAT_COLOUR_DIFFUSE, ColourRGB::White);
        ApplyMaterialColor(materialXML, material, "ColorAmbient", Material::MAT_COLOUR_AMBIENT, ColourRGB(0.1f, 0.1f, 0.1f));
        ApplyMaterialColor(materialXML, material, "ColorSpecular", Material::MAT_COLOUR_SPECULAR, ColourRGB::White);
    }
}

void Scene::ApplyMaterialColor(XMLElement* xmlnode, Material* material, string colorName, Material::eMatColourType type, ColourRGB defaultColor)
{
    ColourRGB color = defaultColor;
    XMLElement* colorXML = xmlnode->FirstChildElement(colorName.c_str());
    if (colorXML)
    {
        color = ReadColourFromXML(colorXML);
    }
    material->SetColour(type, color);
}

void Scene::AddGameComponents(GameObject* go, XMLElement* xmlnode)
{
    // TODO fix me!

    /*
    XMLElement* gameComponents = xmlnode->FirstChildElement("GameComponents");
    if (gameComponents)
    {
        XMLElement* gameComponentXML = gameComponents->FirstChildElement("GameComponent");
        while (gameComponentXML)
        {
            // create & attach component
            int guid = gameComponentXML->IntAttribute("guid");
            GameComponent* comp = CreateComponentByGUID(guid);
            go->AddComponent(comp);

            // set component parameters
            XMLElement* paramXML = gameComponentXML->FirstChildElement("Param");
            while (paramXML)
            {
                SetComponentParameter(guid, comp, paramXML);
                paramXML = paramXML->NextSiblingElement("Param");
            }

            gameComponentXML = gameComponentXML->NextSiblingElement("GameComponent");
        }
    }
    */
}

void Scene::SerializeGlobalSettings(tinyxml2::XMLElement* parentNode, XMLDocument& rootDoc)
{
    // Camera
    XMLNode* cameraNode = parentNode->InsertEndChild(rootDoc.NewElement("Camera"));
    cameraNode->InsertEndChild(WriteVector3ToXML(m_mainCamera.position, "Position", rootDoc));
    cameraNode->InsertEndChild(WriteVector3ToXML(m_mainCamera.direction, "Direction", rootDoc));
    cameraNode->InsertEndChild(WriteVector3ToXML(m_mainCamera.up, "Up", rootDoc));

    // Light
    XMLNode* lightNode = parentNode->InsertEndChild(rootDoc.NewElement("Light"));
    lightNode->InsertEndChild(WriteVector3ToXML(m_light.position, "Position", rootDoc));
    lightNode->InsertEndChild(WriteColourToXML(m_light.color, "Color", rootDoc));
    lightNode->InsertEndChild(WriteFloatToXML(m_light.power, "Power", "value", rootDoc));
}

void Scene::SerializeHierarchy(tinyxml2::XMLElement* parentNode, XMLDocument& rootDoc)
{
    // TODO implement me
}