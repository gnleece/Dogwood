// main.cpp : Defines the entry point for the console application.
//

#include "Engine\Game.h"
#include "TestScene\BuildTestScene.h"

int main(void)
{
    Game::Singleton().Init("Dogwood", 1024, 768);

    GameObject* sceneRoot = BuildTestScene();
    Game::Singleton().Run(sceneRoot);
}