#include "Game.h"

#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

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
    
    // Frame time setup
    m_minFrameTime = 1 / (float)MAX_FPS;
    m_prevFrameEndTime = glfwGetTime();
    m_deltaTime = 0;

    // Game loop!
    while (!m_gameWindow.ShouldClose())
    {
        // Call Update on all active GameObjects
        std::list<GameObject*>::iterator goIter;
        for (goIter = GameObject::ActiveGameObjects.begin(); goIter != GameObject::ActiveGameObjects.end(); goIter++)
        {
            GameObject* GO = *goIter;
            GO->Update(m_deltaTime);
        }

        // Update systems (physics, animation, rendering, etc)
        RenderManager::Singleton().RenderScene(m_rootObject);
        InputManager::Singleton().PollEvents(m_deltaTime);

        UpdateTime();
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

void Game::UpdateTime()
{
    // Calculate the current frame time
    double currentTime = glfwGetTime();
    m_deltaTime = currentTime - m_prevFrameEndTime;

    // If this frame finished faster than expected, sleep for a while so we don't eat CPU
    if (m_deltaTime < m_minFrameTime)
    {
        double sleepTime = m_minFrameTime - m_deltaTime;
        std::this_thread::sleep_for(std::chrono::milliseconds((long)(sleepTime * 1000)));

        currentTime = glfwGetTime();
        m_deltaTime = currentTime - m_prevFrameEndTime;
    }

    m_prevFrameEndTime = currentTime;
}