#include "Camera.h"



Camera::Camera()
{
}

Camera::Camera(float fovY, float aspectRatio, float nearZ, float farZ)
{
	this->fovY = fovY;
	this->aspectRatio = aspectRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;

	UpdateViewMatrix();
	UpdateProjectMatrix();
}


Camera::~Camera()
{
}

XMFLOAT3 Camera::GetPosition() const
{
	float x = XMVectorGetX(cameraPosition);
	float y = XMVectorGetY(cameraPosition);
	float z = XMVectorGetZ(cameraPosition);

	return XMFLOAT3(x, y, z);
}

void Camera::SetAspectRatio(UINT width, UINT height)
{
	aspectRatio 
		= static_cast<float>(width) 
		/ static_cast<float>(height);
}

void Camera::UpdateViewMatrix()
{
	viewMatrix 
		= XMMatrixLookAtLH(cameraPosition, cameraLook, cameraUp);
}

void Camera::UpdateProjectMatrix()
{
	projectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
}
