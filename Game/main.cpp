// main.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "Scene\Scene.h"

int main(void)
{
    Game::Singleton().Init("Katamari.xml");

    // TODO startup scene should be specified in the project file
    Scene scene;
    scene.LoadScene("Assets\\Scenes\\Scene0.xml");
    Game::Singleton().Run(scene.GetRootObject());
}