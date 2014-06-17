// main.cpp : Defines the entry point for the console application.
//

#include "Engine\Game.h"
#include "Engine\Scene\Scene.h"

int main(void)
{
    Game::Singleton().Init("Dogwood", 1024, 768);

    Scene scene("Scenes\\Scene0.xml");
    Game::Singleton().Run(scene.GetRootObject());
}