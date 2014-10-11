// main.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "Scene\Scene.h"

int main(void)
{
    Game::Singleton().Init("Dogwood Engine : Game", 640, 480, "Assets\\Resources.xml");

    Scene scene;
    scene.LoadScene("Assets\\Scenes\\Scene0.xml");
    Game::Singleton().Run(scene.GetRootObject());
}