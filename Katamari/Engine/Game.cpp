#include "Game.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Math\Algebra.h"
#include "Math\Transformations.h"
#include "Rendering\Light.h"
#include "Rendering\Material.h"
#include "Rendering\Mesh.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\ShaderProgram.h"
#include "Rendering\Texture.h"
#include "GameObject.h"
#include "Util.h"

Game::Game(std::string name, int windowWidth, int windowHeight)
{
    // Window setup
    m_gameWindow.Setup(name, windowWidth, windowHeight);

    // Manager setup
    RenderManager::Singleton().Startup(&m_gameWindow);

    // Game loading/setup
    BuildTestScene();
}

void Game::Run()
{
    // Game loop!
    while (!m_gameWindow.ShouldClose())
    {
        // TODO call Update() on all GameObjects

        // Update systems (physics, animation, rendering, etc)
        RenderManager::Singleton().RenderScene(m_rootObject);

        glfwPollEvents();           // TODO this should be in InputManager
    }
}

void Game::Shutdown()
{
    // Game unloading/shutdown
    DeleteTestScene();

    // Manager shutdown
    RenderManager::Singleton().Shutdown();

    // Window cleanup
    m_gameWindow.Destroy();
    exit(EXIT_SUCCESS);
}

void Game::BuildTestScene()
{
    m_rootObject = new GameObject();
    m_rootObject->SetName("root");
    
    // Load default shader program
    ShaderProgram* defaultShader = new ShaderProgram();
    defaultShader->Load("Engine\\Shaders\\VertexShader.glsl", "Engine\\Shaders\\FragmentShader.glsl");

    // Main camera setup
    Camera mainCamera;
    mainCamera.position     = Vector3(0.0, 0.0, 0.0);
    mainCamera.direction    = Vector3(0.0, 0.0, -1.0);
    mainCamera.up           = Vector3(0.0, 1.0, 0.0);
    RenderManager::Singleton().SetCamera(mainCamera);

    // Light setup
    Vector3 lightPosition(0.0f, 0.0f, 0.0f);
    ColourRGB lightColor(1.0f, 1.0f, 1.0f);
    GLfloat lightPower = 15.0f;
    Light light(lightPosition, lightColor, lightPower);
    RenderManager::Singleton().SetLight(light);
    
    // Test textures
    Texture* tex =  new Texture("Engine\\Assets\\test_texture.bmp");
    Texture* tex2 = new Texture("Engine\\Assets\\test_texture2.bmp");

    // Test materials
    Material* mat = new Material();
    mat->SetShader(defaultShader);
    mat->SetTexture(tex);
    mat->SetColour(Material::MAT_COLOUR_DIFFUSE, ColourRGB::Green);
    Material* mat2 = new Material();
    mat2->SetShader(defaultShader);
    mat2->SetColour(Material::MAT_COLOUR_DIFFUSE, ColourRGB::Yellow);

    // Test meshes
    Mesh* cubeMesh = new Mesh("Engine\\Assets\\Models\\cube.obj");
    Mesh* sphereMesh = new Mesh("Engine\\Assets\\Models\\sphere.obj");

    Matrix4x4 trans;
    Matrix4x4 rot;
    Matrix4x4 scale;

    // TODO oh good clean up all these allocations

    // Test objects

    GameObject* cubeGO = new GameObject();
    cubeGO->SetName("cube");
    cubeGO->SetParent(m_rootObject);
    MeshInstance* cubeMeshIns = new MeshInstance(*cubeGO);     // TODO clean up set component
    cubeMeshIns->SetMesh(cubeMesh);
    cubeMeshIns->SetMaterial(mat);
    cubeGO->SetMesh(cubeMeshIns);

    trans = Translation(Vector3(1.2f, -0.5f, -5.0f));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    scale = UniformScaling(0.45f);
    trans = trans*rot*scale;
    cubeGO->SetLocalTransform(trans);

    GameObject* sphereGO = new GameObject();
    sphereGO->SetName("sphere");
    sphereGO->SetParent(m_rootObject);
    MeshInstance* sphereMeshIns = new MeshInstance(*sphereGO);   // TODO clean up set component
    sphereMeshIns->SetMesh(sphereMesh);
    sphereMeshIns->SetMaterial(mat2);
    sphereGO->SetMesh(sphereMeshIns);

    trans = Translation(Vector3(-1.0f, -0.5f, -5.0f));
    rot = Rotation(45, AXIS_Y);
    rot = rot*Rotation(45, AXIS_X);
    scale = UniformScaling(0.8f);
    trans = trans*rot*scale;
    sphereGO->SetLocalTransform(trans);

    //tex.FreeTexture();
    //tex2.FreeTexture();
}

void Game::DeleteTestScene()
{
    delete m_rootObject;
    // TODO iterate through list of gameobjects & delete
}