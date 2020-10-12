#ifdef GRAPHICS_GL

#include "Rendering/Material.h"
#include "Rendering/GL/GLMaterial.h"
#include "Rendering/GL/GLShaderProgram.h"

#include "Rendering/RenderManager.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Texture.h"

Material* Material::Create()
{
    return GLMaterial::Create();
}

void Material::Destroy(Material* material)
{
    delete material;
}

GLMaterial* GLMaterial::Create()
{
    return new GLMaterial();
}

void GLMaterial::SetMesh(MeshInstance* mesh)
{
    m_mesh = mesh;
}

void GLMaterial::SetShader(ShaderProgram* shader)
{
    m_colors.clear();
    m_textures.clear();

    if (shader != NULL)
    {
        m_shader = (GLShaderProgram*)shader;
        if (m_shader != NULL)
        {
            m_positionParamID = m_shader->GetAttributeLocation("position");
            m_normalParamID = m_shader->GetAttributeLocation("normal");
            m_uvParamID = m_shader->GetAttributeLocation("texcoord");
            m_modelID = m_shader->GetUniformLocation("model");
        }
    }
    else
    {
        m_shader = NULL;
    }

}

void GLMaterial::SetColor(string paramName, ColorRGB color)
{
    m_colors[paramName] = color;
}

void GLMaterial::SetTexture(string paramName, Texture* texture)
{
    m_textures[paramName] = texture;
}

MeshInstance* GLMaterial::GetMesh()
{
    return m_mesh;
}

ShaderProgram* GLMaterial::GetShader()
{
    return m_shader;
}

ColorRGB GLMaterial::GetColor(string paramName)
{
    if (m_colors.count(paramName) > 0)
    {
        return m_colors[paramName];
    }
    return ColorRGB::Black;
}

Texture* GLMaterial::GetTexture(string paramName)
{
    if (m_textures.count(paramName) > 0)
    {
        return m_textures[paramName];
    }
    return NULL;
}

unordered_map<string, ColorRGB>& GLMaterial::GetColors()
{
    return m_colors;
}

unordered_map<string, Texture*>& GLMaterial::GetTextures()
{
    return m_textures;
}

void GLMaterial::ApplyMaterial(Transform& transform, GLint posVBO, GLint normVBO, GLint uvVBO, bool useUVs)
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

void GLMaterial::UnapplyMaterial()
{
    DisableAttribArray(m_positionParamID);
    DisableAttribArray(m_normalParamID);
    DisableAttribArray(m_uvParamID);
}

Material* GLMaterial::DeepCopy()
{
    GLMaterial* newMaterial = (GLMaterial*)(GLMaterial::Create());

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

void GLMaterial::SetUniformParam(string paramName, ColorRGB& color)
{
    GLint paramID = m_shader->GetUniformLocation(paramName);
    glUniform3fv(paramID, 1, color.Start());
}

void GLMaterial::SetAttribParam(GLint paramID, GLint buffer, int size)
{
    glEnableVertexAttribArray(paramID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(paramID, size, GL_FLOAT, GL_FALSE, size * sizeof(float), 0);
}

void GLMaterial::DisableAttribArray(GLint paramID)
{
    glDisableVertexAttribArray(paramID);
}

#endif