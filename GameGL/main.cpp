// main.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "GameComponentFactory.h"
#include "Generated\GameComponentBindings.h"
#include "Common/GLGameWindow.h"
#include "Scene\Scene.h"

int main(void)
{
    // TODO load settings from project file again
    int windowWidth = 1024;
    int windowHeight = 576;
    string windowName = "My game - GL";
    auto gameWindow = GLGameWindow(windowName, windowWidth, windowHeight);

    auto componentFactory = MyFactory();

    Game::Singleton().Init("Katamari.xml", &gameWindow, &componentFactory);

    // TODO startup scene should be specified in the project file
    Scene* scene = Scene::Load("Assets\\Scenes\\RollTest.xml");
    Game::Singleton().Run(scene);
}