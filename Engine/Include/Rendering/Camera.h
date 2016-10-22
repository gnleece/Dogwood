#pragma once

#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Rendering\Color.h"

class Camera
{
public:
    Camera();

    Transform&  GetCameraTransform();
    void        SetCameraTransform(Transform& transform);
    void        SetCameraPositionDirectionUp(Vector3 position, Vector3 direction, Vector3 up);
    Transform&  GetViewTransform();
    void        SetViewTransform(Transform& transform);
    Transform&  GetProjectionTransform();

    Vector3&    GetPosition();
    Vector3&    GetDirection();
    Vector3&    GetUp();

    int         GetPixelWidth();
    void        SetPixelWidth(int pixelWidth);
    int         GetPixelHeight();
    void        SetPixelHeight(int pixelHeight);
    float       GetFOV();
    void        SetFOV(float FOV);
    float       GetNearPlane();
    void        SetNearPlane(float nearPlane);
    float       GetFarPlane();
    void        SetFarPlane(float farPlane);
    ColorRGB    GetClearColor();
    void        SetClearColor(ColorRGB clearColor);

    Vector2     WorldToScreenSpace(Vector3 worldPosition);

    bool        IsDirty();
    void        ClearDirtyFlag();

private:
    void        CalculateProjectionTransform();

    Transform   m_cameraTransform;
    Transform   m_viewTransform;            // Always set to the inverse of the camera transform
    Transform   m_projectionTransform;

    int         m_pixelWidth = 640;
    int         m_pixelHeight = 480;
    float       m_FOV = 45.0f;
    float       m_nearPlane = 0.1f;
    float       m_farPlane = 1000.0f;
    ColorRGB    m_clearColor = ColorRGB::Black;

    bool        m_dirty;                    // TODO should be able to remove this
};