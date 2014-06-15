#pragma once

#include <string>

using std::string;

class Scene
{
public:
    void LoadScene(string filename);
    void UnloadScene();
};