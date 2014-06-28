// main.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "Scene\Scene.h"

int main(void)
{
    Game::Singleton().Init("Dogwood Engine : Game", 1024, 768, "Assets\\Resources.xml");

    Scene scene("Assets\\Scenes\\Scene0.xml");
    Game::Singleton().Run(scene.GetRootObject());
}