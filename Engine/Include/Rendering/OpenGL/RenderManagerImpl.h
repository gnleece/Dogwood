#pragma once

#include "Rendering/RenderManager.h"

class DebugDraw;

class RenderManagerImpl : public RenderManager
{
public:
    friend class RenderManager;
    friend class DebugDraw;

    virtual void Startup(int viewportWidth, int viewportHeight);
    virtual void Shutdown();

    virtual void SetRootObject(GameObjectBase* rootObject);
    virtual void SetLight(Light light);

    virtual Camera& GetCamera();
    virtual void SetCamera(Camera& camera);

    virtual void RenderScene();
    virtual void ApplyGlobalParams(ShaderProgram* shader);

    virtual int GetViewportWidth();
    virtual int GetViewportHeight();

    virtual bool SettingsDirty();

    virtual DebugDraw* GetDebugDraw();

    ShaderProgramImpl* GetCommonShader(eCommonShader name);

private:

    void            LoadCommonShaders();

    PlatSpecificRenderer* m_platSpecificRenderer;

    int             m_viewportWidth;
    int             m_viewportHeight;
    Camera          m_camera;                   // TODO support multiple cameras
    Light           m_light;                    // TODO support multiple light sources
    GameObjectBase* m_rootObject;

    bool            m_dirty;

    ShaderProgramImpl*  m_commonShaders[NUM_COMMON_SHADERS];

    DebugDrawImpl*  m_debugDraw;
};