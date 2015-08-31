#include "Game.h"

#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

#include "Input\InputManager.h"
#include "Input\XInputGamePad.h"
#include "Rendering\RenderManager.h"
#include "Scene\ResourceManager.h"
#include "Scene\Scene.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GameProject.h"

void Game::Init(string projectPath, GameComponentFactory* componentFactory)
{
    printf("=============== GAME INIT ===============\n");

    srand(time(NULL));

    // Resource setup
    ResourceManager::Singleton().Startup();

    // Project setup
    GameProject::Singleton().Startup();
    bool success = GameProject::Singleton().Load(projectPath);
    if (!success)
        return;
    GameProject::Singleton().SetRuntimeComponentFactory(componentFactory);

    // Window setup
    int windowWidth, windowHeight;
    GameProject::Singleton().GetResolution(windowWidth, windowHeight);
    m_gameWindow.Setup(GameProject::Singleton().GetName(), windowWidth, windowHeight);

    // Rendering setup
    RenderConfig renderConfig;
    renderConfig.width = windowWidth;
    renderConfig.height = windowHeight;
    RenderManager::Singleton().Startup(renderConfig);

    // Input setup
    InputManager::Singleton().Startup(&m_gameWindow);
    XInputGamepad* xbox360controller = new XInputGamepad(0);            // TODO make this configurable
    InputManager::Singleton().EnableGamePad(xbox360controller, 0);

    // Game Object setup
    GameObjectManager::Singleton().Startup();
}

void Game::Run(Scene& scene)
{
    printf("\n=============== GAME RUN ===============\n");

    m_rootObject = scene.GetRuntimeRootObject();
    RenderManager::Singleton().SetRootObject(m_rootObject);
    
    // Frame time setup
    m_minFrameTime = 1 / (float)MAX_FPS;
    m_prevFrameEndTime = glfwGetTime();
    m_deltaTime = 0;

    // Game loop!
    while (!m_gameWindow.ShouldClose())
    {
        // Game Object update
        GameObjectManager::Singleton().UpdateActiveGameObjects((float)m_deltaTime);

        // Rendering update
        RenderManager::Singleton().RenderScene();

        // Input update
        InputManager::Singleton().PollEvents((float)m_deltaTime);

        UpdateTime();

        m_gameWindow.SwapBuffers();
    }

    Shutdown();
}

void Game::Shutdown()
{
    // Manager shutdown
    GameObjectManager::Singleton().Shutdown();
    GameProject::Singleton().Shutdown();
    ResourceManager::Singleton().Shutdown();
    InputManager::Singleton().Shutdown();
    RenderManager::Singleton().Shutdown();

    // Window cleanup
    m_gameWindow.Destroy();
    exit(EXIT_SUCCESS);
}

void Game::UpdateTime()
{
    // Calculate the current frame time
    float currentTime = glfwGetTime();
    m_deltaTime = currentTime - m_prevFrameEndTime;

    // If this frame finished faster than expected, sleep for a while so we don't eat CPU
    if (m_deltaTime < m_minFrameTime)
    {
        float sleepTime = m_minFrameTime - m_deltaTime;
        std::this_thread::sleep_for(std::chrono::milliseconds((long)(sleepTime * 1000)));

        currentTime = glfwGetTime();
        m_deltaTime = currentTime - m_prevFrameEndTime;
    }

    m_prevFrameEndTime = currentTime;
}