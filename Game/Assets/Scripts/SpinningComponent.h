#pragma once

#include "GameComponent.h"

#include <string>

using std::string;

class SpinningComponent : public GameComponent
{
public:

#pragma region Serializable
    float   Speed = 50;
    string  Label = "";
    bool    EnableSpinning = false;
    int     NumRotations;
#pragma endregion

    void Update(float deltaTime);
};