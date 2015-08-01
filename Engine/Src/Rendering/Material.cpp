#include "Rendering\Material.h"

#include "Rendering\RenderManager.h"
#include "Rendering\ShaderProgram.h"
#include "Rendering\Texture.h"

Material::Material()
{
    m_texture = NULL;
}

Material* Material::DeepCopy()
{
    Material* newMaterial = new Material();

    newMaterial->m_shader = m_shader;
    newMaterial->m_texture = m_texture;

    unordered_map<GLint, ColourRGB>::iterator iter = m_uniformColors.begin();
    for (; iter != m_uniformColors.end(); iter++)
    {
        newMaterial->SetColor(iter->first, iter->second);
    }

    return newMaterial;
}

void Material::SetShader(ShaderProgram* shader)
{
    m_shader = shader;
    m_uniformColors.clear();

    m_positionParamID   = m_shader->GetAttributeLocation("position");
    m_normalParamID     = m_shader->GetAttributeLocation("normal");
    m_uvParamID         = m_shader->GetAttributeLocation("texcoord");
    m_modelID           = m_shader->GetUniformLocation("model");
}

void Material::SetTexture(Texture* texture)
{
    m_texture = texture;
}

void Material::SetColor(GLint paramID, ColourRGB color)
{
    m_uniformColors[paramID] = color;
}

void Material::SetColor(string paramName, ColourRGB color)
{
    GLint paramID = m_shader->GetUniformLocation(paramName);
    SetColor(paramID, color);
}

ShaderProgram* Material::GetShader()
{
    return m_shader;
}

Texture* Material::GetTexture()
{
    return m_texture;
}

ColourRGB Material::GetColor(GLint paramID)
{
    if (m_uniformColors.count(paramID) > 0)
        return m_uniformColors[paramID];
    return ColourRGB::Black;
}

ColourRGB Material::GetColor(string paramName)
{
    GLint paramID = m_shader->GetUniformLocation(paramName);
    return GetColor(paramID);
}

unordered_map<GLint, ColourRGB>& Material::GetColorList()
{
    return m_uniformColors;
}

void Material::ApplyMaterial(GLint posVBO, GLint normVBO, GLint uvVBO, Transform& transform)
{
    // Apply shader
    m_shader->ApplyShader();

    // Bind texture
    if (m_texture == NULL)
    {
        m_texture = Texture::DefaultTexture();
    }
    m_texture->BindTexture();

    // Set uniform color values for shader
    unordered_map<GLint, ColourRGB>::iterator iter = m_uniformColors.begin();
    for (; iter != m_uniformColors.end(); iter++)
    {
        SetUniformParam(iter->first, iter->second);
    }

    // Set vertex values for shader
    SetAttribParam(m_positionParamID,   posVBO, 3);
    SetAttribParam(m_normalParamID,     normVBO, 3);
    SetAttribParam(m_uvParamID,         uvVBO, 2);

    // Set model matrix value for shader        // TODO not sure this should be here
    glUniformMatrix4fv(m_modelID, 1, GL_FALSE, transform.GetMatrix().Transpose().Start());
}

void Material::UnapplyMaterial()
{
    DisableAttribArray(m_positionParamID);
    DisableAttribArray(m_normalParamID);
    DisableAttribArray(m_uvParamID);
}

void Material::SetUniformParam(GLint paramID, ColourRGB& color)
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