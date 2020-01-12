#pragma once
#include <string>

using std::string;

class GraphicsAPI
{
public:
    static GraphicsAPI* Create();

    virtual string GetGraphicsAPIName() = 0;
};
