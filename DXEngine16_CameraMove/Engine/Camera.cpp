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

void Camera::SetPosition(float x, float y, float z)
{
	// �� w = 1, ���� w = 0.
	cameraPosition = XMVectorSet(x, y, z, 1.0f);
	UpdateViewMatrix();
}

void Camera::MoveForward(float direction)
{
	cameraPosition += direction * cameraForward;
}

void Camera::MoveRight(float direction)
{
	cameraPosition += direction * cameraRight;
}

void Camera::MoveUp(float direction)
{
	cameraPosition += direction * cameraUp;
}

void Camera::Yaw(float angle)
{
	yaw += XMConvertToRadians(angle);
}

void Camera::Pitch(float angle)
{
	pitch += XMConvertToRadians(angle);
}

void Camera::UpdateCamera()
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);

	// ������ �� ���� ���͸� ������ ���� ȸ�� ��ĸ�ŭ ȸ�� ��Ű��.
	cameraLook = XMVector3TransformCoord(defaultForward, rotationMatrix);
	cameraLook = XMVector3Normalize(cameraLook);

	// Y ȸ�� �� ����.
	XMMATRIX rotationYTemp = XMMatrixRotationY(yaw);
	//// X ȸ�� �� ����.
	//XMMATRIX rotationXTemp = XMMatrixRotationX(pitch);

	//XMMATRIX rotMatrix = rotationYTemp * rotationXTemp;

	// ī�޶� ���� ���ϱ�.
	// 1��Ī ī�޶�.
	cameraRight = XMVector3TransformCoord(defaultRight, rotationYTemp);
	cameraUp = XMVector3TransformCoord(defaultUp, rotationYTemp);
	cameraForward = XMVector3TransformCoord(defaultForward, rotationYTemp);

	// 3��Ī ī�޶�.
	//cameraRight = XMVector3TransformCoord(defaultRight, rotationMatrix);
	//cameraUp = XMVector3TransformCoord(defaultUp, rotationMatrix);
	//cameraForward = XMVector3TransformCoord(defaultForward, rotationMatrix);

	// ī�޶� �� ���� ����.
	cameraLook = cameraPosition + cameraLook;

	// �� ��� ����.
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraLook, cameraUp);
}

void Camera::UpdateProjectMatrix()
{
	projectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
}
