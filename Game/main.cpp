// main.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "GameComponentFactory.h"
#include "Generated\GameComponentBindings.h"
#include "Scene\Scene.h"

int main(void)
{
    GameComponentFactory* factory = new MyFactory();
    Game::Singleton().Init("Katamari.xml", factory);

    // TODO startup scene should be specified in the project file
    Scene* scene = Scene::Load("Assets\\Scenes\\RollTest.xml");
    Game::Singleton().Run(scene);
}