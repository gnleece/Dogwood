#include "Game.h"

#include <stdlib.h>
#include <stdio.h>

#include "Input\InputManager.h"
#include "Rendering\RenderManager.h"
#include "GameObject.h"

void Game::Init(string name, int windowWidth, int windowHeight)
{
    // Window setup
    m_gameWindow.Setup(name, windowWidth, windowHeight);

    // Manager setup
    RenderManager::Singleton().Startup(&m_gameWindow);
    InputManager::Singleton().Startup(&m_gameWindow);
}

void Game::Run(GameObject* sceneRoot)
{
    m_rootObject = sceneRoot;
    
    // Game loop!
    while (!m_gameWindow.ShouldClose())
    {
        // TODO call Update() on all GameObjects

        // Update systems (physics, animation, rendering, etc)
        RenderManager::Singleton().RenderScene(m_rootObject);

        glfwPollEvents();           // TODO this should be in InputManager
    }

    Shutdown();
}

void Game::Shutdown()
{
    // Manager shutdown
    InputManager::Singleton().Shutdown();
    RenderManager::Singleton().Shutdown();

    // Window cleanup
    m_gameWindow.Destroy();
    exit(EXIT_SUCCESS);
}