#include "Rendering\Material.h"

#include "Rendering\RenderManager.h"
#include "Rendering\ShaderProgram.h"
#include "Rendering\Texture.h"

void Material::SetMesh(MeshInstance* mesh)
{
    m_mesh = mesh;
}

void Material::SetShader(ShaderProgram* shader)
{
    m_shader = shader;
    m_colors.clear();
    m_textures.clear();

    m_positionParamID   = m_shader->GetAttributeLocation("position");
    m_normalParamID     = m_shader->GetAttributeLocation("normal");
    m_uvParamID         = m_shader->GetAttributeLocation("texcoord");
    m_modelID           = m_shader->GetUniformLocation("model");
}

void Material::SetColor(GLint paramID, ColorRGB color)
{
    m_colors[paramID] = color;
}

void Material::SetColor(string paramName, ColorRGB color)
{
    GLint paramID = m_shader->GetUniformLocation(paramName);
    SetColor(paramID, color);
}

void Material::SetTexture(GLint paramID, Texture* texture)
{
    m_textures[paramID] = texture;
}

void Material::SetTexture(string paramName, Texture* texture)
{
    GLint paramID = m_shader->GetUniformLocation(paramName);
    SetTexture(paramID, texture);
}

MeshInstance* Material::GetMesh()
{
    return m_mesh;
}

ShaderProgram* Material::GetShader()
{
    return m_shader;
}

ColorRGB Material::GetColor(GLint paramID)
{
    if (m_colors.count(paramID) > 0)
        return m_colors[paramID];
    return ColorRGB::Black;
}

ColorRGB Material::GetColor(string paramName)
{
    GLint paramID = m_shader->GetUniformLocation(paramName);
    return GetColor(paramID);
}

Texture* Material::GetTexture(GLint paramID)
{
    if (m_textures.count(paramID) > 0)
        return m_textures[paramID];
    return NULL;
}

Texture* Material::GetTexture(string paramName)
{
    GLint paramID = m_shader->GetUniformLocation(paramName);
    return GetTexture(paramID);
}

unordered_map<GLint, ColorRGB>& Material::GetColorList()
{
    return m_colors;
}

unordered_map<GLint, Texture*>& Material::GetTextureList()
{
    return m_textures;
}

void Material::ApplyMaterial(Transform& transform, GLint posVBO, GLint normVBO, GLint uvVBO, bool useUVs)
{
    // Apply shader
    m_shader->ApplyShader();

    // Set color values for shader
    unordered_map<GLint, ColorRGB>::iterator colorIter = m_colors.begin();
    for (; colorIter != m_colors.end(); colorIter++)
    {
        SetUniformParam(colorIter->first, colorIter->second);
    }

    // Bind textures for shader
    unordered_map<GLint, Texture*>::iterator texIter = m_textures.begin();
    for (; texIter != m_textures.end(); texIter++)
    {
        texIter->second->BindTexture();
    }

    // Set vertex values for shader
    SetAttribParam(m_positionParamID,   posVBO, 3);     // TODO should this be done in the shader instead?
    SetAttribParam(m_normalParamID,     normVBO, 3);
    if (useUVs)
    {
        SetAttribParam(m_uvParamID, uvVBO, 2);
    }

    // Set model matrix value for shader                // TODO not sure this should be here
    glUniformMatrix4fv(m_modelID, 1, GL_FALSE, transform.GetMatrix().Transpose().Start());
}

void Material::UnapplyMaterial()
{
    DisableAttribArray(m_positionParamID);
    DisableAttribArray(m_normalParamID);
    DisableAttribArray(m_uvParamID);
}

Material* Material::DeepCopy()
{
    Material* newMaterial = new Material();

    newMaterial->m_shader = m_shader;

    // Copy colors
    unordered_map<GLint, ColorRGB>::iterator colorIter = m_colors.begin();
    for (; colorIter != m_colors.end(); colorIter++)
    {
        newMaterial->SetColor(colorIter->first, colorIter->second);
    }

    // Copy textures
    unordered_map<GLint, Texture*>::iterator texIter = m_textures.begin();
    for (; texIter != m_textures.end(); texIter++)
    {
        newMaterial->SetTexture(texIter->first, texIter->second);
    }

    return newMaterial;
}

void Material::SetUniformParam(GLint paramID, ColorRGB& color)
{
    glUniform3fv(paramID, 1, color.Start());
}

void Material::SetAttribParam(GLint paramID, GLint buffer, int size)
{
    glEnableVertexAttribArray(paramID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(paramID, size, GL_FLOAT, GL_FALSE, size * sizeof(float), 0);
}

void Material::DisableAttribArray(GLint paramID)
{
    glDisableVertexAttribArray(paramID);
}