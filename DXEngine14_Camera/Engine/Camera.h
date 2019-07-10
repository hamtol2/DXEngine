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

	// Setter.
	void SetAspectRatio(UINT width, UINT height);

private:

	void UpdateViewMatrix();
	void UpdateProjectMatrix();

private:

	float fovY;						// 세로 시야각.
	float aspectRatio;				// 종횡비.
	float nearZ;					// 프러스텀 near 거리.
	float farZ;						// 프러스텀 far 거리.

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMVECTOR cameraPosition 
		= XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);
	XMVECTOR cameraLook 
		= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR cameraUp 
		= XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
};