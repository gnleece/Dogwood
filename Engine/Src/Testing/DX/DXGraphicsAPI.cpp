#include "Testing/GraphicsAPI.h"
#include "Testing/DX/DXGraphicsAPI.h"

GraphicsAPI* GraphicsAPI::Create()
{
    return DXGraphicsAPI::Create();
}

DXGraphicsAPI* DXGraphicsAPI::Create()
{
    return new DXGraphicsAPI();
}

string DXGraphicsAPI::GetGraphicsAPIName()
{
    return "DirectX";
}