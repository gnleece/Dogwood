#pragma once

#include "Testing/GraphicsAPI.h"
#include "Testing/DX/DXGraphicsAPI.h"

class DXGraphicsAPI : public GraphicsAPI
{
public:
    static DXGraphicsAPI* Create();

    virtual string GetGraphicsAPIName();
};