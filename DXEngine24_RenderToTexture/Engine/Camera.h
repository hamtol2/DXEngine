#pragma once

#include "DXUtil.h"

using namespace DirectX;

class Camera : public AlignedAllocation<16>
{
public:
	Camera();
	Camera(float fovY, float aspectRatio, float nearZ, float farZ);
	~Camera();

	// Getter.
	float GetAspectRatio() const { return aspectRatio; }
	
	XMMATRIX GetViewMatrix() const { return viewMatrix; }
	XMMATRIX GetProjectionMatrix() const { return projectionMatrix; }

	XMVECTOR GetPositionVector() const { return cameraPosition; }
	XMFLOAT3 GetPosition() const;

	XMVECTOR GetCameraLook() const { return cameraLook; }
	XMVECTOR GetCameraUp() const { return cameraUp; }

	// Setter.
	void SetAspectRatio(UINT width, UINT height);

	// New.
	void SetPosition(float x, float y, float z);

	// ī�޶� �̵� ����.
	void MoveForward(float direction);
	void MoveRight(float direction);
	void MoveUp(float direction);

	// ��(Yaw) Y �� ȸ��.
	void Yaw(float angle);
	// ��ġ(Pitch) X �� ȸ��.
	void Pitch(float angle);

	void UpdateCamera();

private:

	void UpdateViewMatrix();
	void UpdateProjectMatrix();

private:

	float fovY;						// ���� �þ߰�.
	float aspectRatio;				// ��Ⱦ��.
	float nearZ;					// �������� near �Ÿ�.
	float farZ;						// �������� far �Ÿ�.

	float yaw = 0.0f;						// �� ȸ�� ��.
	float pitch = 0.0f;					// ��ġ ȸ�� ��.

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMVECTOR cameraPosition 
		= XMVectorSet(0.0f, 0.0f, -200.0f, 1.0f);
	XMVECTOR cameraLook 
		= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR cameraUp 
		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// New.
	XMVECTOR cameraRight
		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR cameraForward
		= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMVECTOR defaultUp
		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR defaultRight
		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR defaultForward
		= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
};