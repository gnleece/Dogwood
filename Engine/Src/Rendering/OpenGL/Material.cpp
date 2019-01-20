#include "Rendering\Material.h"
#include "Rendering\OpenGL\MaterialImpl.h"

#include "Rendering\RenderManager.h"
#include "Rendering\ShaderProgram.h"
#include "Rendering\Texture.h"

Material* Material::Create()
{
    return new MaterialImpl();
}

void Material::Destroy(Material* material)
{
    delete material;
}

void MaterialImpl::SetMesh(MeshInstance* mesh)
{
    m_mesh = mesh;
}

void MaterialImpl::SetShader(ShaderProgram* shader)
{
    m_shader = shader;
    m_colors.clear();
    m_textures.clear();

    m_positionParamID = m_shader->GetAttributeLocation("position");
    m_normalParamID = m_shader->GetAttributeLocation("normal");
    m_uvParamID = m_shader->GetAttributeLocation("texcoord");
    m_modelID = m_shader->GetUniformLocation("model");
}

void MaterialImpl::SetColor(string paramName, ColorRGB color)
{
    m_colors[paramName] = color;
}

void MaterialImpl::SetTexture(string paramName, Texture* texture)
{
    m_textures[paramName] = texture;
}

MeshInstance* MaterialImpl::GetMesh()
{
    return m_mesh;
}

ShaderProgram* MaterialImpl::GetShader()
{
    return m_shader;
}

ColorRGB MaterialImpl::GetColor(string paramName)
{
    if (m_colors.count(paramName) > 0)
    {
        return m_colors[paramName];
    }
    return ColorRGB::Black;
}

Texture* MaterialImpl::GetTexture(string paramName)
{
    if (m_textures.count(paramName) > 0)
    {
        return m_textures[paramName];
    }
    return NULL;
}

unordered_map<string, ColorRGB>& MaterialImpl::GetColors()
{
    return m_colors;
}

unordered_map<string, Texture*>& MaterialImpl::GetTextures()
{
    return m_textures;
}

void MaterialImpl::ApplyMaterial(Transform& transform, GLint posVBO, GLint normVBO, GLint uvVBO, bool useUVs)
{
    // Apply shader
    m_shader->ApplyShader();

    // Set color values for shader
    unordered_map<string, ColorRGB>::iterator colorIter = m_colors.begin();
    for (; colorIter != m_colors.end(); colorIter++)
    {
        SetUniformParam(colorIter->first, colorIter->second);
    }

    // Bind textures for shader
    unordered_map<string, Texture*>::iterator texIter = m_textures.begin();
    for (; texIter != m_textures.end(); texIter++)
    {
        texIter->second->BindTexture();
    }

    // Set vertex values for shader
    SetAttribParam(m_positionParamID, posVBO, 3);     // TODO should this be done in the shader instead?
    SetAttribParam(m_normalParamID, normVBO, 3);
    if (useUVs)
    {
        SetAttribParam(m_uvParamID, uvVBO, 2);
    }

    // Set model matrix value for shader                // TODO not sure this should be here
    glUniformMatrix4fv(m_modelID, 1, GL_FALSE, transform.GetWorldMatrix().Transpose().Start());
}

void MaterialImpl::UnapplyMaterial()
{
    DisableAttribArray(m_positionParamID);
    DisableAttribArray(m_normalParamID);
    DisableAttribArray(m_uvParamID);
}

Material* MaterialImpl::DeepCopy()
{
    MaterialImpl* newMaterial = (MaterialImpl*)(MaterialImpl::Create());

    newMaterial->m_shader = m_shader;

    // Copy colors
    unordered_map<string, ColorRGB>::iterator colorIter = m_colors.begin();
    for (; colorIter != m_colors.end(); colorIter++)
    {
        newMaterial->SetColor(colorIter->first, colorIter->second);
    }

    // Copy textures
    unordered_map<string, Texture*>::iterator texIter = m_textures.begin();
    for (; texIter != m_textures.end(); texIter++)
    {
        newMaterial->SetTexture(texIter->first, texIter->second);
    }

    return newMaterial;
}

void MaterialImpl::SetUniformParam(string paramName, ColorRGB& color)
{
    GLint paramID = m_shader->GetUniformLocation(paramName);
    glUniform3fv(paramID, 1, color.Start());
}

void MaterialImpl::SetAttribParam(GLint paramID, GLint buffer, int size)
{
    glEnableVertexAttribArray(paramID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(paramID, size, GL_FLOAT, GL_FALSE, size * sizeof(float), 0);
}

void MaterialImpl::DisableAttribArray(GLint paramID)
{
    glDisableVertexAttribArray(paramID);
}