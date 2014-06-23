#pragma once

#include "..\Engine\GameComponent.h"

#include <string>

using std::string;

class SpinningComponent : public GameComponent
{
public:
    float Speed = 50;
    string Label = "";
    bool EnableSpinning = false;

    void Update(float deltaTime);
};