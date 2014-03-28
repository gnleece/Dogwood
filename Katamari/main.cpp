// main.cpp : Defines the entry point for the console application.
//

#include "Engine\Game.h"

int main(void)
{
    Game game("Katamari", 1024, 768);
    game.Run();
    game.Shutdown();
}