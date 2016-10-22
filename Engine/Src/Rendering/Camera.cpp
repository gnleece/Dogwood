#include "Rendering/Camera.h"
#include "Math/Transformations.h"

Camera::Camera()
{
    CalculateProjectionTransform();
}

Transform& Camera::GetCameraTransform()
{
    return m_cameraTransform;
}

void Camera::SetCameraTransform(Transform& transform)
{
    m_cameraTransform.SetLocalMatrix(transform.GetWorldMatrix());
    m_viewTransform.SetLocalMatrix(m_cameraTransform.GetInverseWorldMatrix());
    m_dirty = true;
}

void Camera::SetCameraPositionDirectionUp(Vector3 position, Vector3 direction, Vector3 up)
{
    m_viewTransform.SetLocalMatrix(LookAt(position, direction, up));
    m_cameraTransform.SetLocalMatrix(m_viewTransform.GetInverseWorldMatrix());
    m_dirty = true;
}

Transform& Camera::GetViewTransform()
{
    // TODO fix
    m_viewTransform = Transform(m_cameraTransform.GetInverseWorldMatrix());
    return m_viewTransform;
}

void Camera::SetViewTransform(Transform& transform)
{
    m_viewTransform.SetLocalMatrix(transform.GetWorldMatrix());
    m_cameraTransform.SetLocalMatrix(m_viewTransform.GetInverseWorldMatrix());
    m_dirty = true;
}

Transform& Camera::GetProjectionTransform()
{
    return m_projectionTransform;
}

Vector3& Camera::GetPosition()
{
    return m_cameraTransform.GetWorldPosition();
}

Vector3& Camera::GetDirection()
{
    return m_cameraTransform.GetForward();
}

Vector3& Camera::GetUp()
{
    return m_cameraTransform.GetUp();
}

int Camera::GetPixelWidth()
{
    return m_pixelWidth;
}

void Camera::SetPixelWidth(int pixelWidth)
{
    m_pixelWidth = pixelWidth;
    CalculateProjectionTransform();
    m_dirty = true;
}

int Camera::GetPixelHeight()
{
    return m_pixelHeight;
}

void Camera::SetPixelHeight(int pixelHeight)
{
    m_pixelHeight = pixelHeight;
    CalculateProjectionTransform();
    m_dirty = true;
}

float Camera::GetFOV()
{
    return m_FOV;
}

void Camera::SetFOV(float FOV)
{
    m_FOV = FOV;
    CalculateProjectionTransform();
    m_dirty = true;
}

float Camera::GetNearPlane()
{
    return m_nearPlane;
}

void Camera::SetNearPlane(float nearPlane)
{
    m_nearPlane = nearPlane;
    CalculateProjectionTransform();
    m_dirty = true;
}

float Camera::GetFarPlane()
{
    return m_farPlane;
}

void Camera::SetFarPlane(float farPlane)
{
    m_farPlane = farPlane;
    CalculateProjectionTransform();
    m_dirty = true;
}

ColorRGB Camera::GetClearColor()
{
    return m_clearColor;
}

void Camera::SetClearColor(ColorRGB clearColor)
{
    m_clearColor = clearColor;
    m_dirty = true;
}

Vector2 Camera::WorldToScreenSpace(Vector3 worldPosition)
{
    // TODO the math for this doesn't seem quite right, debug it
    Vector2 screenPos;
    Vector4 pos = (Vector4(worldPosition, 1));
    Vector3 normalizedPosition = ((m_projectionTransform.GetWorldMatrix()*m_viewTransform.GetWorldMatrix())*pos).xyz();
    float x = Clamp(normalizedPosition[0] / normalizedPosition[2], -1.f, 1.f);
    float y = Clamp(normalizedPosition[1] / normalizedPosition[2], -1.f, 1.f);
    screenPos[0] = (x + 1.0f) * m_pixelWidth / 2.0f;
    screenPos[1] = (1.0f - y) * m_pixelHeight / 2.0f;
    return screenPos;
}

bool Camera::IsDirty()
{
    return m_dirty;
}

void Camera::ClearDirtyFlag()
{
    m_dirty = false;
}

void Camera::CalculateProjectionTransform()
{
    float aspect = (float)m_pixelWidth / m_pixelHeight;
    Matrix4x4 projMatrix = PerspectiveProjection(m_FOV, aspect, m_nearPlane, m_farPlane);
    m_projectionTransform.SetLocalMatrix(projMatrix);
}