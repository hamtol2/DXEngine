#include "Camera.h"
#include <math.h>

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

const float Camera::Dot(XMVECTOR& v1, XMVECTOR& v2)
{
	return XMVectorGetX(v1) * XMVectorGetX(v2)
		+ XMVectorGetY(v1) * XMVectorGetY(v2)
		+ XMVectorGetZ(v1) * XMVectorGetZ(v2);
}

const XMMATRIX Camera::CalculateViewMatrix(XMVECTOR& cameraLook, XMVECTOR& cameraPosition, XMVECTOR& cameraUp)
{
	XMVECTOR cameraZ = cameraLook - cameraPosition;
	cameraZ /= XMVector3Length(cameraZ);

	XMVECTOR cameraX = DirectX::XMVector3Cross(cameraUp, cameraZ);
	cameraX /= XMVector3Length(cameraX);

	XMVECTOR cameraY = DirectX::XMVector3Cross(cameraZ, cameraX);
	cameraY /= XMVector3Length(cameraY);

	float posX = Dot(cameraX, cameraPosition);
	float posY = Dot(cameraY, cameraPosition);
	float posZ = Dot(cameraZ, cameraPosition);

	XMMATRIX viewMatrix = XMMATRIX
	(
		XMVectorGetX(cameraX), XMVectorGetX(cameraY), XMVectorGetX(cameraZ), 0,
		XMVectorGetY(cameraX), XMVectorGetY(cameraY), XMVectorGetY(cameraZ), 0,
		XMVectorGetZ(cameraX), XMVectorGetZ(cameraY), XMVectorGetZ(cameraZ), 0,
		-posX, -posY, -posZ, 1
	);

	return viewMatrix;
}

void Camera::UpdateViewMatrix()
{
	//viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraLook, cameraUp);
	viewMatrix = CalculateViewMatrix(cameraLook, cameraPosition, defaultUp);
}

void Camera::UpdateProjectMatrix()
{
	//projectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	projectionMatrix = CalculateProjectionMatrix(fovY, aspectRatio, nearZ, farZ);
}

const XMMATRIX Camera::CalculateProjectionMatrix(float & fovY, float & aspecRatio, float & nearZ, float & farZ)
{
	float H = 1 / tanf(fovY / 2.0f);
	float W = H / aspectRatio;
	float A = farZ / (farZ - nearZ);
	float B = -nearZ * farZ / (farZ - nearZ);

	XMMATRIX projectionMatrix = XMMATRIX
	{
		W, 0.0f, 0.0f, 0.0f,
		0.0f, H, 0.0f, 0.0f,
		0.0f, 0.0f, A, 1.0f,
		0.0f, 0.0f, B, 0.0f
	};

	return projectionMatrix;
}