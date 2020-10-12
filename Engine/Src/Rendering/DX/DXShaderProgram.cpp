#ifdef GRAPHICS_DX

#include "Rendering/ShaderProgram.h"
#include "Rendering/DX/DXShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Rendering\RenderManager.h"

ShaderProgram* ShaderProgram::Create()
{
    return DXShaderProgram::Create();
}

void ShaderProgram::Destroy(ShaderProgram* shader)
{
    delete shader;
}

DXShaderProgram* DXShaderProgram::Create()
{
    return new DXShaderProgram();
}

void DXShaderProgram::Init(string path, ResourceInfo* resourceInfo)
{

}

void DXShaderProgram::ApplyShader()
{

}

void DXShaderProgram::Delete()
{

}

#endif